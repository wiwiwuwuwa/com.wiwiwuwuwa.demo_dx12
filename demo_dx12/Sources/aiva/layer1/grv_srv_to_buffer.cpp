#include <pch.h>
#include <aiva/layer1/grv_srv_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/dict_buffer.h>
#include <aiva/utils/dict_buffer_utils.h>
#include <aiva/utils/layout_buffer.h>
#include <aiva/utils/layout_buffer_utils.h>
#include <aiva/utils/object_utils.h>

aiva::layer1::GrvSrvToBuffer::GrvSrvToBuffer(EngineType const& engine) : AGraphicResourceView{ engine }
{
	InitializeBuffer();
}

aiva::layer1::GrvSrvToBuffer::~GrvSrvToBuffer()
{
	TerminateBuffer();
}

aiva::layer1::GrvSrvToBuffer::BufferElementType& aiva::layer1::GrvSrvToBuffer::Buffer() const
{
	aiva::utils::Asserts::CheckBool(mBuffer, "Shader buffer is not valid");
	return *mBuffer;
}

void aiva::layer1::GrvSrvToBuffer::InitializeBuffer()
{
	mBuffer = aiva::utils::NewObject<BufferElementType>();
	aiva::utils::Asserts::CheckBool(mBuffer, "Shader buffer is not valid");

	mBuffer->OnChanged().connect(boost::bind(&GrvSrvToBuffer::Buffer_OnChanged, this));
}

void aiva::layer1::GrvSrvToBuffer::TerminateBuffer()
{
	aiva::utils::Asserts::CheckBool(mBuffer, "Shader buffer is not valid");

	mBuffer->OnChanged().disconnect(boost::bind(&GrvSrvToBuffer::Buffer_OnChanged, this));
	mBuffer = {};
}

void aiva::layer1::GrvSrvToBuffer::Buffer_OnChanged()
{
	MarkAsChanged();
}

std::shared_ptr<aiva::layer1::GrvSrvToBuffer::ResourceType> aiva::layer1::GrvSrvToBuffer::CreateDefaultInternalResource() const
{
	return aiva::utils::NewObject<GrBuffer>(Engine());
}

void aiva::layer1::GrvSrvToBuffer::RefreshInternalResourceFromSelf(std::shared_ptr<ResourceType> const& aivaResource)
{
	aiva::utils::Asserts::CheckBool(aivaResource, "Aiva resource is not valid");

	auto const& dictBuffer = mBuffer;
	aiva::utils::Asserts::CheckBool(dictBuffer, "Dict buffer is not valid");

	auto const& binary = aiva::utils::DictBufferUtils::SerializeToBinary(dictBuffer);
	aiva::utils::Asserts::CheckBool(!binary.empty());

	auto const& aivaBuffer = std::dynamic_pointer_cast<GrBuffer>(aivaResource);
	aiva::utils::Asserts::CheckBool(aivaBuffer, "Graphic resource doesn't support buffer");

	aivaBuffer->MemoryType(EResourceMemoryType::CpuToGpu);
	aivaBuffer->Size(binary.size());
	aivaBuffer->SupportShaderAtomics(false);
	aivaBuffer->SupportUnorderedAccess(false);

	auto const& directxBuffer = aivaBuffer->InternalResource();
	winrt::check_bool(directxBuffer);

	void* destinationMemory{};
	winrt::check_hresult(directxBuffer->Map(0, nullptr, &destinationMemory));
	aiva::utils::Asserts::CheckBool(destinationMemory);
	aiva::utils::Asserts::CheckBool(memcpy_s(destinationMemory, binary.size(), binary.data(), binary.size()) == 0);
	directxBuffer->Unmap(0, nullptr);
}

aiva::layer1::EDescriptorHeapType aiva::layer1::GrvSrvToBuffer::HeapType() const
{
	return EDescriptorHeapType::CbvSrvUav;
}

aiva::layer1::EResourceViewType aiva::layer1::GrvSrvToBuffer::ViewType() const
{
	return EResourceViewType::Srv;
}

void aiva::layer1::GrvSrvToBuffer::CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination)
{
	auto const& device = Engine().GraphicHardware().Device();
	winrt::check_bool(device);

	auto const& dictBuffer = mBuffer;
	aiva::utils::Asserts::CheckBool(dictBuffer, "Dict buffer is not valid");

	auto const& layoutBuffer = aiva::utils::LayoutBufferUtils::GenerateFrom(mBuffer);
	aiva::utils::Asserts::CheckBool(layoutBuffer, "Layout buffer is not valid");

	auto const& resourceBuffer = std::dynamic_pointer_cast<GrBuffer>(GetInternalResource());
	aiva::utils::Asserts::CheckBool(resourceBuffer, "Graphic resource doesn't support buffer");

	auto const& resourceObject = resourceBuffer->InternalResource();
	winrt::check_bool(resourceObject);

	auto const& resourceObjectDesc = resourceObject->GetDesc();

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

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::GrvSrvToBuffer::CreateDirectxBarriers(bool const active)
{
	auto const& resource = GetInternalResource();
	aiva::utils::Asserts::CheckBool(resource, "Graphic resource is not valid");

	auto const& state = active ? D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_COMMON;
	return resource->PrepareBarriers(state);
}
