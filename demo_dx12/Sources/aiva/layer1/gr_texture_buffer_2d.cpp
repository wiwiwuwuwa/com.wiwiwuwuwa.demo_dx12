#include <pch.h>
#include <aiva/layer1/gr_texture_buffer_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GrTextureBuffer2D::GrTextureBuffer2D(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeLowLevelData();
}

aiva::layer1::GrTextureBuffer2D::~GrTextureBuffer2D()
{
	TerminateLowLevelData();
}

bool aiva::layer1::GrTextureBuffer2D::SupportsShaderAtomics() const
{
	return mSupportsShaderAtomics;
}

aiva::layer1::GrTextureBuffer2D& aiva::layer1::GrTextureBuffer2D::SupportsShaderAtomics(bool const value)
{
	mSupportsShaderAtomics = value;
	mCacheRefresh.MarkAsChanged(EDirtyFlags::All);

	return *this;
}

std::uint64_t aiva::layer1::GrTextureBuffer2D::Width() const
{
	return mWidth;
}

aiva::layer1::GrTextureBuffer2D& aiva::layer1::GrTextureBuffer2D::Width(std::uint64_t const width)
{
	mWidth = width;
	mCacheRefresh.MarkAsChanged(EDirtyFlags::All);

	return *this;
}

std::uint64_t aiva::layer1::GrTextureBuffer2D::Height() const
{
	return mHeight;
}

aiva::layer1::GrTextureBuffer2D& aiva::layer1::GrTextureBuffer2D::Height(std::uint64_t const height)
{
	mHeight = height;
	mCacheRefresh.MarkAsChanged(EDirtyFlags::All);

	return *this;
}

bool aiva::layer1::GrTextureBuffer2D::SupportsMipMapping() const
{
	return mSupportsMipMapping;
}

aiva::layer1::GrTextureBuffer2D& aiva::layer1::GrTextureBuffer2D::SupportsMipMapping(bool const value)
{
	mSupportsMipMapping = value;
	mCacheRefresh.MarkAsChanged(EDirtyFlags::All);

	return *this;
}

aiva::layer1::EGpuResourceBufferFormat aiva::layer1::GrTextureBuffer2D::BufferFormat() const
{
	return mBufferFormat;
}

aiva::layer1::GrTextureBuffer2D& aiva::layer1::GrTextureBuffer2D::BufferFormat(aiva::layer1::EGpuResourceBufferFormat const bufferFormat)
{
	mBufferFormat = bufferFormat;
	mCacheRefresh.MarkAsChanged(EDirtyFlags::All);

	return *this;
}

bool aiva::layer1::GrTextureBuffer2D::SupportsRenderTarget() const
{
	return mSupportsRenderTarget;
}

aiva::layer1::GrTextureBuffer2D& aiva::layer1::GrTextureBuffer2D::SupportsRenderTarget(bool const value)
{
	mSupportsRenderTarget = value;
	mCacheRefresh.MarkAsChanged(EDirtyFlags::All);

	return *this;
}

bool aiva::layer1::GrTextureBuffer2D::SupportsDepthStencil() const
{
	return mSupportsDepthStencil;
}

aiva::layer1::GrTextureBuffer2D& aiva::layer1::GrTextureBuffer2D::SupportsDepthStencil(bool const value)
{
	mSupportsDepthStencil = value;
	mCacheRefresh.MarkAsChanged(EDirtyFlags::All);

	return *this;
}

bool aiva::layer1::GrTextureBuffer2D::SupportsUnorderedAccess() const
{
	return mSupportsUnorderedAccess;
}

aiva::layer1::GrTextureBuffer2D& aiva::layer1::GrTextureBuffer2D::SupportsUnorderedAccess(const bool value)
{
	mSupportsUnorderedAccess = value;
	mCacheRefresh.MarkAsChanged(EDirtyFlags::All);

	return *this;
}

aiva::utils::TEvAction<aiva::layer1::GrTextureBuffer2D::EDirtyFlags>& aiva::layer1::GrTextureBuffer2D::OnFlushCompleted()
{
	return mCacheRefresh.OnFlushCompleted();
}

winrt::com_ptr<ID3D12Resource> const& aiva::layer1::GrTextureBuffer2D::InternalResource()
{
	mCacheRefresh.FlushChanges();

	winrt::com_ptr<ID3D12Resource> internalResource = mInternalResource;
	winrt::check_bool(internalResource);

	return internalResource;
}

void aiva::layer1::GrTextureBuffer2D::InitializeLowLevelData()
{
	mCacheRefresh.OnFlushRequested().connect(boost::bind(&aiva::layer1::GrTextureBuffer2D::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::GrTextureBuffer2D::TerminateLowLevelData()
{
	mCacheRefresh.OnFlushRequested().disconnect(boost::bind(&aiva::layer1::GrTextureBuffer2D::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::GrTextureBuffer2D::RefreshLowLevelData(EDirtyFlags const dirtyFlags)
{
	aiva::utils::Asserts::CheckBool(Width() > 0);
	aiva::utils::Asserts::CheckBool(Height() > 0);
	aiva::utils::Asserts::CheckBool(!SupportsMipMapping() || Width() == Height());
	aiva::utils::Asserts::CheckBool(SupportsRenderTarget() == !aiva::layer1::SupportsDepthStencil(BufferFormat()));
	aiva::utils::Asserts::CheckBool(SupportsDepthStencil() == aiva::layer1::SupportsDepthStencil(BufferFormat()));
	aiva::utils::Asserts::CheckBool(!SupportsRenderTarget() || !SupportsDepthStencil());
	aiva::utils::Asserts::CheckBool(!SupportsDepthStencil() || !SupportsUnorderedAccess());

	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
	heapFlags |= SupportsShaderAtomics() ? D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS : heapFlags;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = Width();
	resourceDesc.Height = Height();
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = SupportsMipMapping() ? 0 : 1;
	resourceDesc.Format = aiva::layer1::ToInternalEnum(BufferFormat());
	resourceDesc.SampleDesc = { 1, 0 };
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Flags |= SupportsRenderTarget() ? D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET : resourceDesc.Flags;
	resourceDesc.Flags |= SupportsDepthStencil() ? D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL : resourceDesc.Flags;
	resourceDesc.Flags |= SupportsUnorderedAccess() ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : resourceDesc.Flags;

	D3D12_RESOURCE_STATES resourceStates = D3D12_RESOURCE_STATE_COMMON;

	winrt::com_ptr<ID3D12Resource> resourceObject{};
	winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&resourceObject)));

	winrt::check_bool(resourceObject);
	mInternalResource = resourceObject;
}
