#include <pch.h>
#include <aiva/layer1/grv_srv_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GrvSrvToBuffer::GrvSrvToBuffer(Engine const& engine, GrvSrvToBufferDesc const& desc) : mEngine{ engine }
{
	Desc(desc);
}

aiva::layer1::GrvSrvToBuffer::~GrvSrvToBuffer()
{
	Desc({});
}

aiva::layer1::GrvSrvToBufferDesc const& aiva::layer1::GrvSrvToBuffer::Desc() const
{
	return mDesc;
}

aiva::layer1::GrvSrvToBuffer& aiva::layer1::GrvSrvToBuffer::Desc(GrvSrvToBufferDesc const& desc)
{
	auto const previousDesc = Desc();
	auto const desiredDesc = desc;

	mDesc = desiredDesc;
	RefreshInternalResourceUpdated(previousDesc, desiredDesc);

	return *this;
}

aiva::layer1::ShaderBuffer& aiva::layer1::GrvSrvToBuffer::Buffer() const
{
	aiva::utils::Asserts::CheckBool(mBuffer);
	return *mBuffer;
}

aiva::layer1::GrvSrvToBuffer& aiva::layer1::GrvSrvToBuffer::ApplyBufferChanges()
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

void aiva::layer1::GrvSrvToBuffer::RefreshBufferData(std::shared_ptr<const ShaderStruct> const& shaderStruct)
{
	aiva::utils::Asserts::CheckBool(shaderStruct);
	mBuffer = ShaderBuffer::Create(shaderStruct);
}

aiva::utils::EvAction& aiva::layer1::GrvSrvToBuffer::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}

void aiva::layer1::GrvSrvToBuffer::RefreshInternalResourceUpdated(GrvSrvToBufferDesc const& previousDesc, GrvSrvToBufferDesc const& desiredDesc)
{
	if (previousDesc.Resource)
	{
		previousDesc.Resource->OnInternalResourceUpdated().disconnect(boost::bind(&GrvSrvToBuffer::NotifyInternalResourceUpdated, this));
	}

	if (desiredDesc.Resource)
	{
		desiredDesc.Resource->OnInternalResourceUpdated().connect(boost::bind(&GrvSrvToBuffer::NotifyInternalResourceUpdated, this));
	}

	NotifyInternalResourceUpdated();
}

void aiva::layer1::GrvSrvToBuffer::NotifyInternalResourceUpdated()
{
	OnInternalResourceUpdated()();
}

std::optional<D3D12_SHADER_RESOURCE_VIEW_DESC> aiva::layer1::GrvSrvToBuffer::InternalResource() const
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

	D3D12_SHADER_RESOURCE_VIEW_DESC directxViewDesc{};
	directxViewDesc.Format = directxResourceDesc.Format;
	directxViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	directxViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	directxViewDesc.Buffer.FirstElement = 0;
	directxViewDesc.Buffer.NumElements = aivaBuffer->Num();
	directxViewDesc.Buffer.StructureByteStride = aivaBuffer->ByteStride();
	directxViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	return directxViewDesc;
}
