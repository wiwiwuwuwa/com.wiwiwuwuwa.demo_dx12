#include <pch.h>
#include <aiva/layer1/grv_uav_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GrvUavToBuffer::GrvUavToBuffer(Engine const& engine, GrvUavToBufferDesc const& desc) : mEngine{ engine }
{
	Desc(desc);
}

aiva::layer1::GrvUavToBuffer::~GrvUavToBuffer()
{
	Desc({});
}

aiva::layer1::GrvUavToBufferDesc const& aiva::layer1::GrvUavToBuffer::Desc() const
{
	return mDesc;
}

aiva::layer1::GrvUavToBuffer& aiva::layer1::GrvUavToBuffer::Desc(GrvUavToBufferDesc const& desc)
{
	auto const previousDesc = Desc();
	auto const desiredDesc = desc;

	mDesc = desiredDesc;
	RefreshInternalResourceUpdated(previousDesc, desiredDesc);

	return *this;
}

aiva::layer1::ShaderBuffer& aiva::layer1::GrvUavToBuffer::Buffer() const
{
	aiva::utils::Asserts::CheckBool(mBuffer);
	return *mBuffer;
}

aiva::layer1::GrvUavToBuffer& aiva::layer1::GrvUavToBuffer::ApplyBufferChanges()
{
	aiva::utils::Asserts::CheckBool(mBuffer);

	auto const& desc = Desc();
	aiva::utils::Asserts::CheckBool(desc.Resource);

	auto const& aivaResource = desc.Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);

	auto const& currentDirectxResource = aivaResource->InternalResource();
	winrt::check_bool(currentDirectxResource);

	auto const& currentDirectxDesc = currentDirectxResource->GetDesc();
	aiva::utils::Asserts::CheckBool(currentDirectxDesc.Width > 0);

	auto const& binaryData = mBuffer->SerializeToBinary();
	aiva::utils::Asserts::CheckBool(binaryData.size() > 0);

	auto const& needUpdateAivaDesc = currentDirectxDesc.Width != binaryData.size();
	if (needUpdateAivaDesc)
	{
		auto updatedAivaDesc = aivaResource->Desc();
		updatedAivaDesc.Size = binaryData.size();

		aivaResource->Desc(updatedAivaDesc);
	}

	auto const& updatedDirectxResource = aivaResource->InternalResource();
	winrt::check_bool(updatedDirectxResource);

	void* destinationMemory{};
	winrt::check_hresult(updatedDirectxResource->Map(0, nullptr, &destinationMemory));
	aiva::utils::Asserts::CheckBool(destinationMemory);
	aiva::utils::Asserts::CheckBool(memcpy_s(destinationMemory, binaryData.size(), binaryData.data(), binaryData.size()) == 0);
	updatedDirectxResource->Unmap(0, nullptr);

	return *this;
}

void aiva::layer1::GrvUavToBuffer::RefreshBufferData(std::shared_ptr<const ShaderStruct> const& shaderStruct)
{
	aiva::utils::Asserts::CheckBool(shaderStruct);
	mBuffer = ShaderBuffer::Create(shaderStruct);
}

aiva::utils::EvAction& aiva::layer1::GrvUavToBuffer::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}

void aiva::layer1::GrvUavToBuffer::RefreshInternalResourceUpdated(GrvUavToBufferDesc const& previousDesc, GrvUavToBufferDesc const& desiredDesc)
{
	if (previousDesc.Resource)
	{
		previousDesc.Resource->OnInternalResourceUpdated().disconnect(boost::bind(&GrvUavToBuffer::NotifyInternalResourceUpdated, this));
	}

	if (desiredDesc.Resource)
	{
		desiredDesc.Resource->OnInternalResourceUpdated().connect(boost::bind(&GrvUavToBuffer::NotifyInternalResourceUpdated, this));
	}

	NotifyInternalResourceUpdated();
}

void aiva::layer1::GrvUavToBuffer::NotifyInternalResourceUpdated()
{
	OnInternalResourceUpdated()();
}

std::optional<D3D12_UNORDERED_ACCESS_VIEW_DESC> aiva::layer1::GrvUavToBuffer::InternalResource() const
{
	auto const& aivaViewDesc = Desc();
	if (!aivaViewDesc.Resource)
	{
		return {};
	}

	auto const& aivaBuffer = mBuffer;
	aiva::utils::Asserts::CheckBool(aivaBuffer);

	auto const& aivaResource = aivaViewDesc.Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);

	auto const& aivaResourceDesc = aivaResource->Desc();
	aiva::utils::Asserts::CheckBool(aivaResourceDesc.SupportUnorderedAccess);

	auto const& directxResource = aivaResource->InternalResource();
	winrt::check_bool(directxResource);

	auto const& directxResourceDesc = directxResource->GetDesc();

	D3D12_UNORDERED_ACCESS_VIEW_DESC directxViewDesc{};
	directxViewDesc.Format = directxResourceDesc.Format;
	directxViewDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	directxViewDesc.Buffer.FirstElement = 0;
	directxViewDesc.Buffer.NumElements = aivaBuffer->Num();
	directxViewDesc.Buffer.StructureByteStride = aivaBuffer->ByteStride();
	directxViewDesc.Buffer.CounterOffsetInBytes = 0;
	directxViewDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

	return directxViewDesc;
}
