#include <pch.h>
#include <aiva/layer1/grv_srv_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/object_factory.h>

aiva::layer1::GrvSrvToBuffer::GrvSrvToBuffer(EngineType const& engine) : AGraphicResourceView{ engine }
{
	InitializeBuffer();
}

aiva::layer1::GrvSrvToBuffer::~GrvSrvToBuffer()
{
	TerminateBuffer();
}

aiva::layer1::GrvSrvToBuffer::BufferType& aiva::layer1::GrvSrvToBuffer::Buffer() const
{
	aiva::utils::Asserts::CheckBool(mBuffer, "Shader buffer is not valid");
	return *mBuffer;
}

void aiva::layer1::GrvSrvToBuffer::InitializeBuffer()
{
	mBuffer = BufferType::FactoryType::Create<BufferType>();
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

void aiva::layer1::GrvSrvToBuffer::RefreshInternalResourceFromSelf(std::shared_ptr<ResourceType> const& aivaResource)
{
	auto const& binary = Buffer().SerializeToBinary();
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

	auto const& buffer = std::dynamic_pointer_cast<GrBuffer>(InternalResource());
	aiva::utils::Asserts::CheckBool(buffer, "Graphic resource doesn't support buffer");

	auto const& resource = buffer->InternalResource();
	winrt::check_bool(resource);

	auto const& resourceDesc = resource->GetDesc();

	auto viewDesc = D3D12_SHADER_RESOURCE_VIEW_DESC{};
	viewDesc.Format = resourceDesc.Format;
	viewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	viewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	viewDesc.Buffer.FirstElement = 0;
	viewDesc.Buffer.NumElements = Buffer().Num();
	viewDesc.Buffer.StructureByteStride = Buffer().ByteStride();
	viewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	device->CreateShaderResourceView(resource.get(), &viewDesc, destination);
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::GrvSrvToBuffer::CreateDirectxBarriers(bool const active)
{
	auto const& resource = InternalResource();
	aiva::utils::Asserts::CheckBool(resource, "Graphic resource is not valid");

	auto const& state = active ? D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_COMMON;
	return resource->PrepareBarriers(state);
}
