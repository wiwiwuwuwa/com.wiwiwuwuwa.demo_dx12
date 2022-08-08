#include <pch.h>
#include <aiva/layer1/grv_cbv_to_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/object_utils.h>

aiva::layer1::GrvCbvToBuffer::GrvCbvToBuffer(EngineType const& engine) : AGraphicResourceView{ engine }
{
	InitializeStruct();
}

aiva::layer1::GrvCbvToBuffer::~GrvCbvToBuffer()
{
	TerminateStruct();
}

aiva::layer1::GrvCbvToBuffer::StructType& aiva::layer1::GrvCbvToBuffer::Struct() const
{
	aiva::utils::Asserts::CheckBool(mStruct, "Shader struct is not valid");
	return *mStruct;
}

void aiva::layer1::GrvCbvToBuffer::InitializeStruct()
{
	mStruct = aiva::utils::NewObject<StructType>();
	aiva::utils::Asserts::CheckBool(mStruct, "Shader struct is not valid");

	mStruct->OnChanged().connect(boost::bind(&GrvCbvToBuffer::Struct_OnChanged, this));
}

void aiva::layer1::GrvCbvToBuffer::TerminateStruct()
{
	aiva::utils::Asserts::CheckBool(mStruct, "Shader struct is not valid");

	mStruct->OnChanged().disconnect(boost::bind(&GrvCbvToBuffer::Struct_OnChanged, this));
	mStruct = {};
}

void aiva::layer1::GrvCbvToBuffer::Struct_OnChanged()
{
	MarkAsChanged();
}

void aiva::layer1::GrvCbvToBuffer::RefreshInternalResourceFromSelf(std::shared_ptr<ResourceType> const& aivaResource)
{
	auto const& binary = (*aiva::utils::NewObject<ShaderBuffer>()).Struct(mStruct).Add(mStruct).SerializeToBinary();
	aiva::utils::Asserts::CheckBool(!binary.empty(), "Binary data is empty");

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

	auto const& buffer = std::dynamic_pointer_cast<GrBuffer>(InternalResource());
	aiva::utils::Asserts::CheckBool(buffer, "Graphic resource doesn't support buffer");

	auto const& resource = buffer->InternalResource();
	winrt::check_bool(resource);

	auto const& resourceDesc = resource->GetDesc();

	auto viewDesc = D3D12_CONSTANT_BUFFER_VIEW_DESC{};
	viewDesc.BufferLocation = resource->GetGPUVirtualAddress();
	viewDesc.SizeInBytes = resourceDesc.Width;

	device->CreateConstantBufferView(&viewDesc, destination);
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::GrvCbvToBuffer::CreateDirectxBarriers(bool const active)
{
	auto const& resource = InternalResource();
	aiva::utils::Asserts::CheckBool(resource, "Graphic resource is not valid");

	auto const& state = active ? D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE : D3D12_RESOURCE_STATE_COMMON;
	return resource->PrepareBarriers(state);
}
