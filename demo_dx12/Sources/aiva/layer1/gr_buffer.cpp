#include <pch.h>
#include <aiva/layer1/gr_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GrBuffer::GrBuffer(Engine const& engine, GrBufferDescriptor const& descriptor) : mEngine{ engine }
{
	SetDescriptor(descriptor);
}

aiva::layer1::GrBuffer::~GrBuffer()
{

}

aiva::layer1::GrBufferDescriptor aiva::layer1::GrBuffer::GetDescriptor() const
{
	winrt::com_ptr<ID3D12Resource> const resource = mInternalResource;
	winrt::check_bool(resource);

	GrBufferDescriptor descriptor{};

	{
		D3D12_HEAP_PROPERTIES heapProperties{};
		D3D12_HEAP_FLAGS heapFlags{};
		winrt::check_hresult(resource->GetHeapProperties(&heapProperties, &heapFlags));

		descriptor.MemoryType = FromInternalEnum(heapProperties.Type);
		descriptor.SupportShaderAtomics = heapFlags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS;
	}

	{
		D3D12_RESOURCE_DESC resourceDesc = resource->GetDesc();

		descriptor.Size = resourceDesc.Width;
		descriptor.SupportUnorderedAccess = resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	return descriptor;
}

aiva::layer1::GrBuffer& aiva::layer1::GrBuffer::SetDescriptor(GrBufferDescriptor const& descriptor)
{
	UpdateInternalResource(descriptor);
	return *this;
}

winrt::com_ptr<ID3D12Resource> const& aiva::layer1::GrBuffer::InternalResource()
{
	winrt::com_ptr<ID3D12Resource> const resource = mInternalResource;
	winrt::check_bool(resource);

	return resource;
}

aiva::utils::EvAction& aiva::layer1::GrBuffer::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}

void aiva::layer1::GrBuffer::UpdateInternalResource(GrBufferDescriptor const& descriptor)
{
	aiva::utils::Asserts::CheckBool(!(descriptor.SupportShaderAtomics && descriptor.MemoryType != EGpuResourceMemoryType::GpuOnly));
	aiva::utils::Asserts::CheckBool(descriptor.Size > 0);

	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = ToInternalEnum(descriptor.MemoryType);
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
	heapFlags |= descriptor.SupportShaderAtomics ? D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS : heapFlags;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = descriptor.Size;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc = { 1, 0 };
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Flags |= descriptor.SupportUnorderedAccess ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : resourceDesc.Flags;

	D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON;
	resourceStates |= descriptor.MemoryType == EGpuResourceMemoryType::CpuToGpu ? D3D12_RESOURCE_STATE_GENERIC_READ : resourceStates;
	resourceStates |= descriptor.MemoryType == EGpuResourceMemoryType::GpuToCpu ? D3D12_RESOURCE_STATE_COPY_DEST : resourceStates;

	winrt::com_ptr<ID3D12Resource> resource{};
	winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&resource)));

	winrt::check_bool(resource);
	mInternalResource = resource;

	OnInternalResourceUpdated()();
}
