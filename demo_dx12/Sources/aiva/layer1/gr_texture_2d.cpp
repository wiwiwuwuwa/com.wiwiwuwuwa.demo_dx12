#include <pch.h>
#include <aiva/layer1/gr_texture_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::GrTexture2D::GrTexture2D(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
	InitializeInternalResources();
}

aiva::layer1::GrTexture2D::GrTexture2D(Engine const& engine, GrTexture2DDesc const& desc) : GrTexture2D(engine)
{
	Desc(desc, true);
}

aiva::layer1::GrTexture2D::GrTexture2D(Engine const& engine, winrt::com_ptr<ID3D12Resource> const& resource) : GrTexture2D(engine)
{
	winrt::check_bool(resource);

	Desc(resource, false);
	InternalResource(resource);
}

aiva::layer1::GrTexture2D::~GrTexture2D()
{
	TerminateInternalResources();
	TerminateCacheUpdater();
}

aiva::layer1::GrTexture2D::CacheUpdaterType& aiva::layer1::GrTexture2D::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::GrTexture2D::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::GrTexture2D::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

std::optional<aiva::layer1::GrTexture2DDesc> const& aiva::layer1::GrTexture2D::Desc() const
{
	return mDesc;
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::Desc(std::optional<GrTexture2DDesc> const& desc)
{
	return Desc(desc, true);
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::Desc(std::optional<GrTexture2DDesc> const& desc, bool const markAsChanged)
{
	mDesc = desc;

	if (markAsChanged)
	{
		CacheUpdater().MarkAsChanged();
	}

	return *this;
}

void aiva::layer1::GrTexture2D::InitializeInternalResources()
{
	CacheUpdater().FlushExecutors().connect(boost::bind(&GrTexture2D::RefreshInternalResources, this));
}

void aiva::layer1::GrTexture2D::TerminateInternalResources()
{
	CacheUpdater().FlushExecutors().disconnect(boost::bind(&GrTexture2D::RefreshInternalResources, this));
}

void aiva::layer1::GrTexture2D::RefreshInternalResources()
{
	mInternalResource = Desc() ? CreateInternalResource(mEngine, *Desc()) : decltype(mInternalResource){};
}

winrt::com_ptr<ID3D12Resource> aiva::layer1::GrTexture2D::CreateInternalResource(Engine const& engine, GrTexture2DDesc const& desc)
{
	aiva::utils::Asserts::CheckBool(desc.Width > 0);
	aiva::utils::Asserts::CheckBool(desc.Height > 0);
	aiva::utils::Asserts::CheckBool(desc.SupportDepthStencil == SupportsDepthStencil(desc.BufferFormat));
	aiva::utils::Asserts::CheckBool(!(desc.SupportMipMapping && desc.Width != desc.Height));
	aiva::utils::Asserts::CheckBool(desc.SupportRenderTarget != SupportsDepthStencil(desc.BufferFormat));
	aiva::utils::Asserts::CheckBool(!(desc.SupportRenderTarget && desc.SupportDepthStencil));
	aiva::utils::Asserts::CheckBool(!(desc.SupportDepthStencil && desc.SupportUnorderedAccess));

	auto const& device = engine.GraphicHardware().Device();
	winrt::check_bool(device);

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
	heapFlags |= desc.SupportShaderAtomics ? D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS : heapFlags;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = desc.Width;
	resourceDesc.Height = desc.Height;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = desc.SupportMipMapping ? 0 : 1;
	resourceDesc.Format = ToInternalEnum(desc.BufferFormat);
	resourceDesc.SampleDesc = { 1, 0 };
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Flags |= desc.SupportRenderTarget ? D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET : resourceDesc.Flags;
	resourceDesc.Flags |= desc.SupportDepthStencil ? D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL : resourceDesc.Flags;
	resourceDesc.Flags |= desc.SupportUnorderedAccess ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : resourceDesc.Flags;

	D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON;

	winrt::com_ptr<ID3D12Resource> resource{};
	winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&resource)));

	winrt::check_bool(resource);
	return resource;
}

winrt::com_ptr<ID3D12Resource> const aiva::layer1::GrTexture2D::InternalResource()
{
	CacheUpdater().FlushChanges();
	return mInternalResource;
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::InternalResource(winrt::com_ptr<ID3D12Resource> const& resource)
{
	mInternalResource = resource;
}
