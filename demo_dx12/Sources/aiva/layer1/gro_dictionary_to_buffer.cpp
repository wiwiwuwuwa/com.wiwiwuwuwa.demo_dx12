#include <pch.h>
#include <aiva/layer1/gro_dictionary_to_buffer.h>

#include <aiva/layer1/constant_packer.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GroDictionaryToBuffer::GroDictionaryToBuffer(Engine const& engine, GroDictionaryToBufferDesc const& desc) : mEngine{ engine }
{
	InitializeUpdater();
	Desc(desc);
}

aiva::layer1::GroDictionaryToBuffer::~GroDictionaryToBuffer()
{
	Desc({});
	TerminateUpdater();
}

aiva::layer1::GroDictionaryToBufferDesc aiva::layer1::GroDictionaryToBuffer::Desc() const
{
	return mDesc;
}

aiva::layer1::GroDictionaryToBuffer& aiva::layer1::GroDictionaryToBuffer::Desc(GroDictionaryToBufferDesc const& desc)
{
	mDesc = desc;
	return *this;
}

aiva::utils::ActionBatcher& aiva::layer1::GroDictionaryToBuffer::GetBatcher()
{
	return mBatcher;
}

void aiva::layer1::GroDictionaryToBuffer::InitializeUpdater()
{
	GetBatcher().OnEndBatch().connect(boost::bind(&GroDictionaryToBuffer::UpdateInternalResources, this));
}

void aiva::layer1::GroDictionaryToBuffer::TerminateUpdater()
{
	GetBatcher().OnEndBatch().disconnect(boost::bind(&GroDictionaryToBuffer::UpdateInternalResources, this));
}

void aiva::layer1::GroDictionaryToBuffer::UpdateInternalResources()
{
	auto const aivaOperatorDesc = Desc();
	if (!aivaOperatorDesc.Resource)
	{
		return;
	}

	auto const aivaResource = aivaOperatorDesc.Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);

	auto const device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto const currentDirectXResource = aivaResource->InternalResource();
	winrt::check_bool(currentDirectXResource);

	auto const currentDirectXResourceDesc = currentDirectXResource->GetDesc();
	aiva::utils::Asserts::CheckBool(currentDirectXResourceDesc.Width > 0);

	auto const binaryData = ConstantPacker::PackConstants(mValues.cbegin(), mValues.cend());
	aiva::utils::Asserts::CheckBool(!binaryData.empty());

	auto const needUpdateResource = currentDirectXResourceDesc.Width != binaryData.size();
	if (needUpdateResource)
	{
		GrBufferDesc updatedAivaResourceDesc = aivaResource->Desc();
		updatedAivaResourceDesc.Size = binaryData.size();

		aivaResource->Desc(updatedAivaResourceDesc);
	}

	auto const updatedDirectXResource = aivaResource->InternalResource();
	winrt::check_bool(updatedDirectXResource);

	void* destinationMemory{};
	winrt::check_hresult(updatedDirectXResource->Map(0, nullptr, &destinationMemory));
	aiva::utils::Asserts::CheckBool(destinationMemory);
	aiva::utils::Asserts::CheckBool(memcpy_s(destinationMemory, binaryData.size(), binaryData.data(), binaryData.size()) == 0);
	updatedDirectXResource->Unmap(0, nullptr);
}
