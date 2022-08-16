#include <pch.h>
#include <aiva/layer1/grv_cbv_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/dict_buffer.h>
#include <aiva/utils/dict_buffer_utils.h>
#include <aiva/utils/dict_struct.h>
#include <aiva/utils/dict_struct_utils.h>
#include <aiva/utils/enum_utils.h>
#include <aiva/utils/meta_struct.h>
#include <aiva/utils/meta_struct_utils.h>
#include <aiva/utils/object_utils.h>

aiva::layer1::GrvCbvToBuffer::GrvCbvToBuffer(EngineType const& engine) : AGraphicResourceView{ engine }
{
	InitializeStruct();
}

aiva::layer1::GrvCbvToBuffer::~GrvCbvToBuffer()
{
	TerminateStruct();
}

aiva::layer1::GrvCbvToBuffer::StructElementType& aiva::layer1::GrvCbvToBuffer::Struct() const
{
	aiva::utils::Asserts::CheckBool(mStruct, "Shader struct is not valid");
	return *mStruct;
}

void aiva::layer1::GrvCbvToBuffer::InitializeStruct()
{
	mStruct = aiva::utils::NewObject<StructElementType>();
	aiva::utils::Asserts::CheckBool(mStruct, "Shader struct is not valid");

	mStruct->OnCacheDataChanged().connect(boost::bind(&GrvCbvToBuffer::Struct_OnChanged, this));
}

void aiva::layer1::GrvCbvToBuffer::TerminateStruct()
{
	aiva::utils::Asserts::CheckBool(mStruct, "Shader struct is not valid");

	mStruct->OnCacheDataChanged().disconnect(boost::bind(&GrvCbvToBuffer::Struct_OnChanged, this));
	mStruct = {};
}

void aiva::layer1::GrvCbvToBuffer::Struct_OnChanged()
{
	MarkCacheDataAsChanged(EGrvCacheFlags::BufferBin);
}

std::shared_ptr<aiva::layer1::GrvCbvToBuffer::ResourceType> aiva::layer1::GrvCbvToBuffer::CreateDefaultInternalResource() const
{
	return aiva::utils::NewObject<GrBuffer>(Engine());
}

void aiva::layer1::GrvCbvToBuffer::RefreshInternalResourceFromSelf(std::shared_ptr<ResourceType> const& aivaResource, EGrvCacheFlags const dirtyFlags)
{
	using namespace aiva::utils;

	Asserts::CheckBool(aivaResource, "Aiva resource is not valid");

	auto const dictBuffer = aiva::utils::NewObject<BufferElementType>();
	dictBuffer->Layout(mStruct);
	dictBuffer->Add(mStruct);

	auto const binaryData = aiva::utils::DictBufferUtils::SerializeToBinary(dictBuffer);
	Asserts::CheckBool(!std::empty(binaryData), "Binary data is empty");

	auto const& aivaBuffer = std::dynamic_pointer_cast<GrBuffer>(aivaResource);
	Asserts::CheckBool(aivaBuffer, "Graphic resource doesn't support buffer");

	auto needRefreshPtr = false;
	needRefreshPtr |= (EnumUtils::Has(dirtyFlags, EGrvCacheFlags::BufferPtr));
	needRefreshPtr |= (EnumUtils::Has(dirtyFlags, EGrvCacheFlags::BufferBin) && aivaBuffer->Size() != std::size(binaryData));
	needRefreshPtr |= (aivaBuffer->MemoryType() != EResourceMemoryType::CpuToGpu);
	needRefreshPtr |= (aivaBuffer->SupportShaderAtomics() != false);
	needRefreshPtr |= (aivaBuffer->SupportUnorderedAccess() != false);
	needRefreshPtr ? RefreshInternalResourcePtr(aivaBuffer, binaryData) : [] {};

	auto needRefreshBin = false;
	needRefreshBin |= (needRefreshPtr);
	needRefreshBin |= (EnumUtils::Has(dirtyFlags, EGrvCacheFlags::BufferBin));
	needRefreshBin ? RefreshInternalResourceBin(aivaBuffer, binaryData) : [] {};
}

void aiva::layer1::GrvCbvToBuffer::RefreshInternalResourcePtr(std::shared_ptr<GrBuffer> const& aivaResource, std::vector<std::byte> const& binaryData) const
{
	using namespace aiva::utils;

	Asserts::CheckBool(aivaResource, "Aiva resource is not valid");
	Asserts::CheckBool(!std::empty(binaryData), "Binary data is empty");

	aivaResource->MemoryType(EResourceMemoryType::CpuToGpu);
	aivaResource->Size(std::size(binaryData));
	aivaResource->SupportShaderAtomics(false);
	aivaResource->SupportUnorderedAccess(false);
}

void aiva::layer1::GrvCbvToBuffer::RefreshInternalResourceBin(std::shared_ptr<GrBuffer> const& aivaResource, std::vector<std::byte> const& binaryData) const
{
	using namespace aiva::utils;

	Asserts::CheckBool(aivaResource, "Aiva resource is not valid");
	Asserts::CheckBool(!std::empty(binaryData), "Binary data is empty");

	auto const& directxBuffer = aivaResource->GetInternalResource();
	winrt::check_bool(directxBuffer);

	void* destinationMemory{};
	winrt::check_hresult(directxBuffer->Map(0, nullptr, &destinationMemory));
	Asserts::CheckBool(destinationMemory);
	Asserts::CheckBool(memcpy_s(destinationMemory, std::size(binaryData), std::data(binaryData), std::size(binaryData)) == 0);
	directxBuffer->Unmap(0, nullptr);
}

aiva::layer1::EDescriptorHeapType aiva::layer1::GrvCbvToBuffer::HeapType() const
{
	return EDescriptorHeapType::CbvSrvUav;
}

aiva::layer1::EResourceViewType aiva::layer1::GrvCbvToBuffer::ViewType() const
{
	return EResourceViewType::Cbv;
}

void aiva::layer1::GrvCbvToBuffer::CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination)
{
	auto const& device = Engine().GraphicHardware().Device();
	winrt::check_bool(device);

	auto const& buffer = std::dynamic_pointer_cast<GrBuffer>(GetInternalResource());
	aiva::utils::Asserts::CheckBool(buffer, "Graphic resource doesn't support buffer");

	auto const& resource = buffer->GetInternalResource();
	winrt::check_bool(resource);

	auto const& resourceDesc = resource->GetDesc();

	auto viewDesc = D3D12_CONSTANT_BUFFER_VIEW_DESC{};
	viewDesc.BufferLocation = resource->GetGPUVirtualAddress();
	viewDesc.SizeInBytes = resourceDesc.Width;

	device->CreateConstantBufferView(&viewDesc, destination);
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::GrvCbvToBuffer::CreateDirectxBarriers(bool const active)
{
	auto const& resource = GetInternalResource();
	aiva::utils::Asserts::CheckBool(resource, "Graphic resource is not valid");

	auto const& state = active ? D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_COMMON;
	return resource->CreateDirectxBarriers(state);
}
