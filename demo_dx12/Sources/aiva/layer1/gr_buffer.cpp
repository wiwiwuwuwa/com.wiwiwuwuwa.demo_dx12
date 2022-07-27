#include <pch.h>
#include <aiva/layer1/gr_buffer.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::GrBuffer::GrBuffer(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
	InitializeInternalResources();
}

aiva::layer1::GrBuffer::~GrBuffer()
{
	TerminateInternalResources();
	TerminateCacheUpdater();
}

aiva::layer1::GrBuffer::CacheUpdaterType& aiva::layer1::GrBuffer::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::GrBuffer::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::GrBuffer::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

std::optional<aiva::layer1::GrBufferDesc> const& aiva::layer1::GrBuffer::Desc() const
{
	return mDesc;
}

aiva::layer1::GrBuffer& aiva::layer1::GrBuffer::Desc(std::optional<GrBufferDesc> const& desc)
{
	mDesc = desc;
	CacheUpdater().MarkAsChanged();

	return *this;
}

void aiva::layer1::GrBuffer::InitializeInternalResources()
{
	CacheUpdater().FlushExecutors().connect(boost::bind(&GrBuffer::RefreshInternalResources, this));
}

void aiva::layer1::GrBuffer::TerminateInternalResources()
{
	CacheUpdater().FlushExecutors().disconnect(boost::bind(&GrBuffer::RefreshInternalResources, this));
}

void aiva::layer1::GrBuffer::RefreshInternalResources()
{
	mInternalResource = Desc() ? CreateInternalResource(mEngine, *Desc()) : decltype(mInternalResource){};
}

winrt::com_ptr<ID3D12Resource> aiva::layer1::GrBuffer::CreateInternalResource(Engine const& engine, GrBufferDesc const& desc)
{
	aiva::utils::Asserts::CheckBool(!(desc.SupportShaderAtomics && desc.MemoryType != EGpuResourceMemoryType::GpuOnly));
	aiva::utils::Asserts::CheckBool(desc.Size > 0);

	auto const& device = engine.GraphicHardware().Device();
	winrt::check_bool(device);

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = ToInternalEnum(desc.MemoryType);
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
	heapFlags |= desc.SupportShaderAtomics ? D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS : heapFlags;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = desc.Size;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc = { 1, 0 };
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Flags |= desc.SupportUnorderedAccess ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : resourceDesc.Flags;

	D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON;
	resourceStates |= desc.MemoryType == EGpuResourceMemoryType::CpuToGpu ? D3D12_RESOURCE_STATE_GENERIC_READ : resourceStates;
	resourceStates |= desc.MemoryType == EGpuResourceMemoryType::GpuToCpu ? D3D12_RESOURCE_STATE_COPY_DEST : resourceStates;

	winrt::com_ptr<ID3D12Resource> resource{};
	winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&resource)));

	winrt::check_bool(resource);
	return resource;
}

winrt::com_ptr<ID3D12Resource> const aiva::layer1::GrBuffer::InternalResource()
{
	CacheUpdater().FlushChanges();
	return mInternalResource;
}
