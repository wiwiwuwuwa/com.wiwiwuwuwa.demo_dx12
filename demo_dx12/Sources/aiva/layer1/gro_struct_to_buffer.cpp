#include <pch.h>
#include <aiva/layer1/gro_struct_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GroStructToBuffer::GroStructToBuffer(Engine const& engine, GroStructToBufferDesc const& desc) : mEngine{ engine }
{
	InitializeStruct();
	Desc(desc);
}

aiva::layer1::GroStructToBuffer::~GroStructToBuffer()
{
	Desc({});
	TerminateStruct();
}

aiva::layer1::GroStructToBufferDesc const& aiva::layer1::GroStructToBuffer::Desc() const
{
	return mDesc;
}

aiva::layer1::GroStructToBuffer& aiva::layer1::GroStructToBuffer::Desc(GroStructToBufferDesc const& desc)
{
	auto const previousDesc = Desc();
	auto const desiredDesc = desc;

	mDesc = desiredDesc;
	RefreshInternalResourceUpdated(previousDesc, desiredDesc);

	return *this;
}

aiva::layer1::ShaderStruct& aiva::layer1::GroStructToBuffer::Struct() const
{
	aiva::utils::Asserts::CheckBool(mStruct);
	return *mStruct;
}

aiva::layer1::GroStructToBuffer& aiva::layer1::GroStructToBuffer::ApplyChanges()
{
	aiva::utils::Asserts::CheckBool(mStruct);

	auto const& desc = Desc();
	aiva::utils::Asserts::CheckBool(desc.Resource);

	auto const& aivaResource = desc.Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);

	auto const& currentDirectxResource = aivaResource->InternalResource();
	winrt::check_bool(currentDirectxResource);

	auto const& currentDirectxDesc = currentDirectxResource->GetDesc();
	aiva::utils::Asserts::CheckBool(currentDirectxDesc.Width > 0);

	auto const& binaryData = (*ShaderBuffer::Create(mStruct)).Add(mStruct).SerializeToBinary();
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

void aiva::layer1::GroStructToBuffer::InitializeStruct()
{
	mStruct = ShaderStruct::Create();
}

void aiva::layer1::GroStructToBuffer::TerminateStruct()
{
	mStruct = {};
}

aiva::utils::EvAction& aiva::layer1::GroStructToBuffer::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}

void aiva::layer1::GroStructToBuffer::RefreshInternalResourceUpdated(GroStructToBufferDesc const& previousDesc, GroStructToBufferDesc const& desiredDesc)
{
	if (previousDesc.Resource)
	{
		previousDesc.Resource->OnInternalResourceUpdated().disconnect(boost::bind(&GroStructToBuffer::NotifyInternalResourceUpdated, this));
	}

	if (desiredDesc.Resource)
	{
		desiredDesc.Resource->OnInternalResourceUpdated().connect(boost::bind(&GroStructToBuffer::NotifyInternalResourceUpdated, this));
	}

	NotifyInternalResourceUpdated();
}

void aiva::layer1::GroStructToBuffer::NotifyInternalResourceUpdated()
{
	OnInternalResourceUpdated()();
}
