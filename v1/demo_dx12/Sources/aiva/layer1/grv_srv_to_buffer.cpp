#include <pch.h>
#include <aiva/layer1/grv_srv_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/shader_consts.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/dict_buffer.h>
#include <aiva/utils/dict_buffer_utils.h>
#include <aiva/utils/layout_buffer.h>
#include <aiva/utils/layout_buffer_utils.h>
#include <aiva/utils/object_utils.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	GrvSrvToBuffer::GrvSrvToBuffer(EngineType const& engine) : AGraphicResourceView{ engine }
	{
		InitializeBuffer();
	}

	GrvSrvToBuffer::~GrvSrvToBuffer()
	{
		TerminateBuffer();
	}

	GrvSrvToBuffer::BufferElementType& GrvSrvToBuffer::Buffer() const
	{
		Asserts::CheckBool(mBuffer, "Shader buffer is not valid");
		return *mBuffer;
	}

	void GrvSrvToBuffer::InitializeBuffer()
	{
		mBuffer = NewObject<BufferElementType>();
		Asserts::CheckBool(mBuffer, "Shader buffer is not valid");

		mBuffer->OnCacheDataChanged().connect(boost::bind(&ThisType::Buffer_OnChanged, this));
	}

	void GrvSrvToBuffer::TerminateBuffer()
	{
		Asserts::CheckBool(mBuffer, "Shader buffer is not valid");

		mBuffer->OnCacheDataChanged().disconnect(boost::bind(&ThisType::Buffer_OnChanged, this));
		mBuffer = {};
	}

	void GrvSrvToBuffer::Buffer_OnChanged()
	{
		MarkCacheDataAsChanged(CacheFlagType::BufferBin);
	}

	GrvSrvToBuffer::ParentType::ResourceTypeShared GrvSrvToBuffer::CreateDefaultInternalResource() const
	{
		auto const resource = NewObject<ResourceType>(Engine());
		Asserts::CheckBool(resource, "Resource is not valid");

		resource->Size(ShaderConsts::HEAP_SIZE);
		return resource;
	}

	void GrvSrvToBuffer::RefreshInternalResourceFromSelf(ParentType::ResourceTypeShared const& aivaResource, CacheFlagType const dirtyFlags)
	{
		Asserts::CheckBool(aivaResource, "Aiva resource is not valid");

		auto const& dictBuffer = mBuffer;
		Asserts::CheckBool(dictBuffer, "Dict buffer is not valid");

		auto const binaryData = DictBufferUtils::SerializeToBinary(dictBuffer);
		Asserts::CheckBool(!std::empty(binaryData), "Binary data is empty");

		auto const aivaBuffer = std::dynamic_pointer_cast<ResourceType>(aivaResource);
		Asserts::CheckBool(aivaBuffer, "Graphic resource doesn't support buffer");

		auto needRefreshPtr = false;
		needRefreshPtr |= (EnumUtils::Has(dirtyFlags, CacheFlagType::BufferPtr));
		needRefreshPtr |= (EnumUtils::Has(dirtyFlags, CacheFlagType::BufferBin) && aivaBuffer->Size() != std::size(binaryData));
		needRefreshPtr |= (aivaBuffer->MemoryType() != ResourceType::MemoryTypeEnum::CpuToGpu);
		needRefreshPtr |= (aivaBuffer->SupportShaderAtomics() != false);
		needRefreshPtr |= (aivaBuffer->SupportUnorderedAccess() != false);
		needRefreshPtr ? RefreshInternalResourcePtr(aivaBuffer, binaryData) : [] {};

		auto needRefreshBin = false;
		needRefreshBin |= (needRefreshPtr);
		needRefreshBin |= (EnumUtils::Has(dirtyFlags, CacheFlagType::BufferBin));
		needRefreshBin ? RefreshInternalResourceBin(aivaBuffer, binaryData) : [] {};
	}

	void GrvSrvToBuffer::RefreshInternalResourcePtr(ResourceTypeShared const& aivaResource, std::vector<std::byte> const& binaryData) const
	{
		Asserts::CheckBool(aivaResource, "Aiva resource is not valid");
		Asserts::CheckBool(!std::empty(binaryData), "Binary data is empty");

		aivaResource->MemoryType(ResourceType::MemoryTypeEnum::CpuToGpu);
		aivaResource->Size(std::size(binaryData));
		aivaResource->SupportShaderAtomics(false);
		aivaResource->SupportUnorderedAccess(false);
	}

	void GrvSrvToBuffer::RefreshInternalResourceBin(ResourceTypeShared const& aivaResource, std::vector<std::byte> const& binaryData) const
	{
		Asserts::CheckBool(aivaResource, "Aiva resource is not valid");
		Asserts::CheckBool(!std::empty(binaryData), "Binary data is empty");

		auto const& directxBuffer = aivaResource->GetInternalResource();
		winrt::check_bool(directxBuffer);

		void* destinationMemory{};
		winrt::check_hresult(directxBuffer->Map(0, nullptr, &destinationMemory));
		Asserts::CheckBool(destinationMemory, "Destination memory is not valid");
		Asserts::CheckBool(memcpy_s(destinationMemory, std::size(binaryData), std::data(binaryData), std::size(binaryData)) == 0, "Failed to memcpy_s");
		directxBuffer->Unmap(0, nullptr);
	}

	GrvSrvToBuffer::HeapTypeEnum GrvSrvToBuffer::HeapType() const
	{
		return HeapTypeEnum::CbvSrvUav;
	}

	GrvSrvToBuffer::ViewTypeEnum GrvSrvToBuffer::ViewType() const
	{
		return ViewTypeEnum::Srv;
	}

	void GrvSrvToBuffer::CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination)
	{
		auto const& device = Engine().GraphicHardware().Device();
		winrt::check_bool(device);

		auto const& dictBuffer = mBuffer;
		Asserts::CheckBool(dictBuffer, "Dict buffer is not valid");

		auto const layoutBuffer = LayoutBufferUtils::GenerateFrom(mBuffer);
		Asserts::CheckBool(layoutBuffer, "Layout buffer is not valid");

		auto const resourceBuffer = std::dynamic_pointer_cast<GrBuffer>(GetInternalResource());
		Asserts::CheckBool(resourceBuffer, "Graphic resource doesn't support buffer");

		auto const& resourceObject = resourceBuffer->GetInternalResource();
		winrt::check_bool(resourceObject);

		auto const resourceObjectDesc = resourceObject->GetDesc();

		auto viewDesc = D3D12_SHADER_RESOURCE_VIEW_DESC{};
		viewDesc.Format = resourceObjectDesc.Format;
		viewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		viewDesc.Buffer.FirstElement = 0;
		viewDesc.Buffer.NumElements = Buffer().Num();
		viewDesc.Buffer.StructureByteStride = layoutBuffer->Stride();
		viewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

		device->CreateShaderResourceView(resourceObject.get(), &viewDesc, destination);
	}

	std::vector<D3D12_RESOURCE_BARRIER> GrvSrvToBuffer::CreateDirectxBarriers(bool const active)
	{
		auto const& resource = GetInternalResource();
		Asserts::CheckBool(resource, "Graphic resource is not valid");

		auto const state = active ? D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_COMMON;
		return resource->CreateDirectxBarriers(state);
	}
}
