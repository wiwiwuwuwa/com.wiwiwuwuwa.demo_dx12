#include <pch.h>
#include <aiva/layer1/grv_cbv_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/shader_consts.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/dict_buffer.h>
#include <aiva/utils/dict_buffer_utils.h>
#include <aiva/utils/dict_struct.h>
#include <aiva/utils/dict_struct_utils.h>
#include <aiva/utils/enum_utils.h>
#include <aiva/utils/meta_struct.h>
#include <aiva/utils/meta_struct_utils.h>
#include <aiva/utils/object_utils.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	GrvCbvToBuffer::GrvCbvToBuffer(EngineType const& engine) : AGraphicResourceView{ engine }
	{
		InitializeStruct();
	}

	GrvCbvToBuffer::~GrvCbvToBuffer()
	{
		TerminateStruct();
	}

	GrvCbvToBuffer::StructElementType& GrvCbvToBuffer::Struct() const
	{
		Asserts::CheckBool(mStruct, "Shader struct is not valid");
		return *mStruct;
	}

	void GrvCbvToBuffer::InitializeStruct()
	{
		mStruct = NewObject<StructElementType>();
		Asserts::CheckBool(mStruct, "Shader struct is not valid");

		mStruct->OnCacheDataChanged().connect(boost::bind(&ThisType::Struct_OnChanged, this));
	}

	void GrvCbvToBuffer::TerminateStruct()
	{
		Asserts::CheckBool(mStruct, "Shader struct is not valid");

		mStruct->OnCacheDataChanged().disconnect(boost::bind(&ThisType::Struct_OnChanged, this));
		mStruct = {};
	}

	void GrvCbvToBuffer::Struct_OnChanged()
	{
		MarkCacheDataAsChanged(CacheFlagType::BufferBin);
	}

	GrvCbvToBuffer::ParentType::ResourceTypeShared GrvCbvToBuffer::CreateDefaultInternalResource() const
	{
		auto const resource = NewObject<ResourceType>(Engine());
		Asserts::CheckBool(resource, "Resource is not valid");

		resource->Size(ShaderConsts::HEAP_SIZE);
		return resource;
	}

	void GrvCbvToBuffer::RefreshInternalResourceFromSelf(ParentType::ResourceTypeShared const& aivaResource, CacheFlagType const dirtyFlags)
	{
		Asserts::CheckBool(aivaResource, "Aiva resource is not valid");

		auto const dictBuffer = NewObject<BufferElementType>();
		Asserts::CheckBool(dictBuffer, "Dict buffer is not valid");

		dictBuffer->Layout(mStruct);
		dictBuffer->Add(mStruct);

		auto const binaryData = DictBufferUtils::SerializeToBinary(dictBuffer);
		Asserts::CheckBool(!std::empty(binaryData), "Binary data is empty");

		auto const aivaBuffer = std::dynamic_pointer_cast<GrBuffer>(aivaResource);
		Asserts::CheckBool(aivaBuffer, "Graphic resource doesn't support buffer");

		auto needRefreshPtr = false;
		needRefreshPtr |= (EnumUtils::Has(dirtyFlags, EGrvCacheFlags::BufferPtr));
		needRefreshPtr |= (EnumUtils::Has(dirtyFlags, EGrvCacheFlags::BufferBin) && aivaBuffer->Size() != std::size(binaryData));
		needRefreshPtr |= (aivaBuffer->MemoryType() != ResourceType::MemoryTypeEnum::CpuToGpu);
		needRefreshPtr |= (aivaBuffer->SupportShaderAtomics() != false);
		needRefreshPtr |= (aivaBuffer->SupportUnorderedAccess() != false);
		needRefreshPtr ? RefreshInternalResourcePtr(aivaBuffer, binaryData) : [] {};

		auto needRefreshBin = false;
		needRefreshBin |= (needRefreshPtr);
		needRefreshBin |= (EnumUtils::Has(dirtyFlags, EGrvCacheFlags::BufferBin));
		needRefreshBin ? RefreshInternalResourceBin(aivaBuffer, binaryData) : [] {};
	}

	void GrvCbvToBuffer::RefreshInternalResourcePtr(ResourceTypeShared const& aivaResource, std::vector<std::byte> const& binaryData) const
	{
		Asserts::CheckBool(aivaResource, "Aiva resource is not valid");
		Asserts::CheckBool(!std::empty(binaryData), "Binary data is empty");

		aivaResource->MemoryType(ResourceType::MemoryTypeEnum::CpuToGpu);
		aivaResource->Size(std::size(binaryData));
		aivaResource->SupportShaderAtomics(false);
		aivaResource->SupportUnorderedAccess(false);
	}

	void GrvCbvToBuffer::RefreshInternalResourceBin(ResourceTypeShared const& aivaResource, std::vector<std::byte> const& binaryData) const
	{
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

	GrvCbvToBuffer::HeapTypeEnum GrvCbvToBuffer::HeapType() const
	{
		return HeapTypeEnum::CbvSrvUav;
	}

	GrvCbvToBuffer::ViewTypeEnum GrvCbvToBuffer::ViewType() const
	{
		return ViewTypeEnum::Cbv;
	}

	void GrvCbvToBuffer::CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination)
	{
		auto const& device = Engine().GraphicHardware().Device();
		winrt::check_bool(device);

		auto const buffer = std::dynamic_pointer_cast<GrBuffer>(GetInternalResource());
		Asserts::CheckBool(buffer, "Graphic resource doesn't support buffer");

		auto const& resource = buffer->GetInternalResource();
		winrt::check_bool(resource);

		auto const resourceDesc = resource->GetDesc();

		auto viewDesc = D3D12_CONSTANT_BUFFER_VIEW_DESC{};
		viewDesc.BufferLocation = resource->GetGPUVirtualAddress();
		viewDesc.SizeInBytes = resourceDesc.Width;

		device->CreateConstantBufferView(&viewDesc, destination);
	}

	std::vector<D3D12_RESOURCE_BARRIER> GrvCbvToBuffer::CreateDirectxBarriers(bool const active)
	{
		auto const& resource = GetInternalResource();
		Asserts::CheckBool(resource, "Graphic resource is not valid");

		auto const state = active ? D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_COMMON;
		return resource->CreateDirectxBarriers(state);
	}
}
