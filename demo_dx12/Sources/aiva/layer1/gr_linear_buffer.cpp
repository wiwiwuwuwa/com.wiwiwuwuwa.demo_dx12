#include <pch.h>
#include <aiva/layer1/gr_linear_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GrLinearBuffer::GrLinearBuffer(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeLowLevelData();
}

aiva::layer1::GrLinearBuffer::~GrLinearBuffer()
{
	TerminateLowLevelData();
}

aiva::layer1::EGpuResourceMemoryType aiva::layer1::GrLinearBuffer::MemoryType() const
{
	return mMemoryType;
}

bool aiva::layer1::GrLinearBuffer::SupportsShaderAtomics() const
{
	return mSupportsShaderAtomics;
}

std::uint32_t aiva::layer1::GrLinearBuffer::Size() const
{
	return mSize;
}

bool aiva::layer1::GrLinearBuffer::SupportsUnorderedAccess() const
{
	return mSupportsUnorderedAccess;
}

aiva::utils::TEvAction<aiva::layer1::GrLinearBuffer::EDirtyFlags>& aiva::layer1::GrLinearBuffer::OnFlushCompleted()
{
	return mCacheRefresh.OnFlushCompleted();
}

winrt::com_ptr<ID3D12Resource> const& aiva::layer1::GrLinearBuffer::InternalResource()
{
	mCacheRefresh.FlushChanges();

	winrt::com_ptr<ID3D12Resource> internalResource = mInternalResource;
	winrt::check_bool(internalResource);

	return internalResource;
}

void aiva::layer1::GrLinearBuffer::InitializeLowLevelData()
{
	mCacheRefresh.OnFlushRequested().connect(boost::bind(&aiva::layer1::GrLinearBuffer::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::GrLinearBuffer::TerminateLowLevelData()
{
	mCacheRefresh.OnFlushRequested().disconnect(boost::bind(&aiva::layer1::GrLinearBuffer::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::GrLinearBuffer::RefreshLowLevelData(EDirtyFlags const dirtyFlags)
{
	aiva::utils::Asserts::CheckBool(!(SupportsShaderAtomics() && MemoryType() != EGpuResourceMemoryType::GpuOnly));
	aiva::utils::Asserts::CheckBool(Size() > 0);

	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = ToInternalEnum(MemoryType());
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask;

	D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
	heapFlags |= SupportsShaderAtomics() ? D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS : heapFlags;

	D3D12_RESOURCE_DESC resourceDesc{};
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
	resourceDesc.Flags |= SupportsUnorderedAccess() ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : resourceDesc.Flags;

	D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON;
	resourceStates |= MemoryType() == EGpuResourceMemoryType::CpuToGpu ? D3D12_RESOURCE_STATE_GENERIC_READ : resourceStates;
	resourceStates |= MemoryType() == EGpuResourceMemoryType::GpuToCpu ? D3D12_RESOURCE_STATE_COPY_DEST : resourceStates;

	winrt::com_ptr<ID3D12Resource> resourceObject{};
	winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&resourceObject)));

	winrt::check_bool(resourceObject);
	mInternalResource = resourceObject;
}
