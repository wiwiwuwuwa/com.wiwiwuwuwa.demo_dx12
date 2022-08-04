#include <pch.h>
#include <aiva/layer1/gr_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/resource_barrier.h>

aiva::layer1::GrBuffer::GrBuffer(aiva::layer1::Engine const& engine) : AGraphicResource(engine)
{

}

aiva::layer1::EResourceMemoryType aiva::layer1::GrBuffer::MemoryType() const
{
	return mMemoryType;
}

aiva::layer1::GrBuffer& aiva::layer1::GrBuffer::MemoryType(EResourceMemoryType const memoryType)
{
	if (mMemoryType != memoryType)
	{
		mMemoryType = memoryType;
		MarkAsChanged();
	}
	
	return *this;
}

std::size_t aiva::layer1::GrBuffer::Size() const
{
	return mSize;
}

aiva::layer1::GrBuffer& aiva::layer1::GrBuffer::Size(std::size_t const size)
{
	if (mSize != size)
	{
		mSize = size;
		MarkAsChanged();
	}

	return *this;
}

bool aiva::layer1::GrBuffer::SupportShaderAtomics() const
{
	return mSupportShaderAtomics;
}

aiva::layer1::GrBuffer& aiva::layer1::GrBuffer::SupportShaderAtomics(bool const support)
{
	if (mSupportShaderAtomics != support)
	{
		mSupportShaderAtomics = support;
		MarkAsChanged();
	}

	return *this;
}

bool aiva::layer1::GrBuffer::SupportUnorderedAccess() const
{
	return mSupportUnorderedAccess;
}

aiva::layer1::GrBuffer& aiva::layer1::GrBuffer::SupportUnorderedAccess(bool const support)
{
	if (mSupportUnorderedAccess != support)
	{
		mSupportUnorderedAccess = support;
		MarkAsChanged();
	}

	return *this;
}

void aiva::layer1::GrBuffer::RefreshInternalResourceFromSelf(winrt::com_ptr<ID3D12Resource>& resource, aiva::utils::ResourceBarrier& barrier)
{
	aiva::utils::Asserts::CheckBool(!(SupportShaderAtomics() && MemoryType() != EResourceMemoryType::GpuOnly), "Shader atomics is not supported");
	aiva::utils::Asserts::CheckBool(Size() > 0, "Size is not valid");
	
	auto const& device = Engine().GraphicHardware().Device();
	winrt::check_bool(device);
	
	auto heapProperties = D3D12_HEAP_PROPERTIES{};
	heapProperties.Type = ToInternalEnum(MemoryType());
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;
	
	auto heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
	heapFlags |= SupportShaderAtomics() ? D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS : heapFlags;
	
	auto resourceDesc = D3D12_RESOURCE_DESC{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = Size();
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc = { 1, 0 };
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Flags |= SupportUnorderedAccess() ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : resourceDesc.Flags;
	
	auto resourceStates = D3D12_RESOURCE_STATE_COMMON;
	resourceStates |= MemoryType() == EResourceMemoryType::CpuToGpu ? D3D12_RESOURCE_STATE_GENERIC_READ : resourceStates;
	resourceStates |= MemoryType() == EResourceMemoryType::GpuToCpu ? D3D12_RESOURCE_STATE_COPY_DEST : resourceStates;
	barrier = resourceStates;
	
	winrt::com_ptr<ID3D12Resource> commitedResource{};
	winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&commitedResource)));
	resource = commitedResource;
}

void aiva::layer1::GrBuffer::RefreshSelfFromInternalResource(winrt::com_ptr<ID3D12Resource> const& resource)
{
	winrt::check_bool(resource);

	auto heapProperties = D3D12_HEAP_PROPERTIES{};
	auto heapFlags = D3D12_HEAP_FLAGS{};
	winrt::check_hresult(resource->GetHeapProperties(&heapProperties, &heapFlags));

	MemoryType(FromInternalEnum(heapProperties.Type));
	SupportShaderAtomics(heapFlags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS);

	auto const resourceDesc = resource->GetDesc();
	aiva::utils::Asserts::CheckBool(resourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER, "Resource is not a buffer");

	Size(resourceDesc.Width);
	SupportUnorderedAccess(resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
}
