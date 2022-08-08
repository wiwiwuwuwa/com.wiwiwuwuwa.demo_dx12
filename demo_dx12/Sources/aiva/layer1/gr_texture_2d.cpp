#include <pch.h>
#include <aiva/layer1/gr_texture_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/resource_barrier.h>

aiva::layer1::GrTexture2D::GrTexture2D(EngineType const& engine) : AGraphicResource(engine)
{

}

aiva::layer1::GrTexture2D::~GrTexture2D()
{

}

aiva::layer1::EResourceBufferFormat aiva::layer1::GrTexture2D::Format() const
{
	return mFormat;
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::Format(EResourceBufferFormat const format)
{
	if (mFormat != format)
	{
		mFormat = format;
		MarkAsChanged();
	}

	return *this;
}

std::size_t aiva::layer1::GrTexture2D::Width() const
{
	return mWidth;
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::Width(std::size_t const width)
{
	if (mWidth != width)
	{
		mWidth = width;
		MarkAsChanged();
	}

	return *this;
}

std::size_t aiva::layer1::GrTexture2D::Height() const
{
	return mHeight;
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::Height(std::size_t const height)
{
	if (mHeight != height)
	{
		mHeight = height;
		MarkAsChanged();
	}

	return *this;
}

bool aiva::layer1::GrTexture2D::SupportDepthStencil() const
{
	return mSupportDepthStencil;
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::SupportDepthStencil(bool const support)
{
	if (mSupportDepthStencil != support)
	{
		mSupportDepthStencil = support;
		MarkAsChanged();
	}

	return *this;
}

bool aiva::layer1::GrTexture2D::SupportMipMapping() const
{
	return mSupportMipMapping;
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::SupportMipMapping(bool const support)
{
	if (mSupportMipMapping != support)
	{
		mSupportMipMapping = support;
		MarkAsChanged();
	}

	return *this;
}

bool aiva::layer1::GrTexture2D::SupportRenderTarget() const
{
	return mSupportRenderTarget;
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::SupportRenderTarget(bool const support)
{
	if (mSupportRenderTarget != support)
	{
		mSupportRenderTarget = support;
		MarkAsChanged();
	}

	return *this;
}

bool aiva::layer1::GrTexture2D::SupportShaderAtomics() const
{
	return mSupportShaderAtomics;
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::SupportShaderAtomics(bool const support)
{
	if (mSupportShaderAtomics != support)
	{
		mSupportShaderAtomics = support;
		MarkAsChanged();
	}

	return *this;
}

bool aiva::layer1::GrTexture2D::SupportUnorderedAccess() const
{
	return mSupportUnorderedAccess;
}

aiva::layer1::GrTexture2D& aiva::layer1::GrTexture2D::SupportUnorderedAccess(bool const support)
{
	if (mSupportUnorderedAccess != support)
	{
		mSupportUnorderedAccess = support;
		MarkAsChanged();
	}

	return *this;
}

void aiva::layer1::GrTexture2D::RefreshInternalResourceFromSelf(winrt::com_ptr<ID3D12Resource>& resource, aiva::utils::ResourceBarrier& barrier)
{
	aiva::utils::Asserts::CheckBool(Width() > 0);
	aiva::utils::Asserts::CheckBool(Height() > 0);
	aiva::utils::Asserts::CheckBool(SupportDepthStencil() == SupportsDepthStencil(Format()));
	aiva::utils::Asserts::CheckBool(!(SupportMipMapping() && Width() != Height()));
	aiva::utils::Asserts::CheckBool(SupportRenderTarget() != SupportsDepthStencil(Format()));
	aiva::utils::Asserts::CheckBool(!(SupportRenderTarget() && SupportDepthStencil()));
	aiva::utils::Asserts::CheckBool(!(SupportDepthStencil() && SupportUnorderedAccess()));

	auto const& device = Engine().GraphicHardware().Device();
	winrt::check_bool(device);

	auto heapProperties = D3D12_HEAP_PROPERTIES{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	auto heapFlags = D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
	heapFlags |= SupportShaderAtomics() ? D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS : heapFlags;

	auto resourceDesc = D3D12_RESOURCE_DESC{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = Width();
	resourceDesc.Height = Height();
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = SupportMipMapping() ? 0 : 1;
	resourceDesc.Format = ToInternalEnum(Format());
	resourceDesc.SampleDesc = { 1, 0 };
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Flags |= SupportRenderTarget() ? D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET : resourceDesc.Flags;
	resourceDesc.Flags |= SupportDepthStencil() ? D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL : resourceDesc.Flags;
	resourceDesc.Flags |= SupportUnorderedAccess() ? D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS : resourceDesc.Flags;

	auto resourceStates = D3D12_RESOURCE_STATE_COMMON;
	barrier = resourceStates;

	auto commitedResource = winrt::com_ptr<ID3D12Resource>{};
	winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&commitedResource)));

	winrt::check_bool(commitedResource);
	resource = commitedResource;
}

void aiva::layer1::GrTexture2D::RefreshSelfFromInternalResource(winrt::com_ptr<ID3D12Resource> const& resource)
{
	winrt::check_bool(resource);

	auto heapProperties = D3D12_HEAP_PROPERTIES{};
	auto heapFlags = D3D12_HEAP_FLAGS{};
	winrt::check_hresult(resource->GetHeapProperties(&heapProperties, &heapFlags));

	SupportShaderAtomics(heapFlags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS);

	auto const& resourceDesc = resource->GetDesc();
	aiva::utils::Asserts::CheckBool(resourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D, "Resource is not a texture 2D");

	Format(FromInternalEnum(resourceDesc.Format));
	Width(resourceDesc.Width);
	Height(resourceDesc.Height);
	SupportDepthStencil(resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	SupportMipMapping(resourceDesc.MipLevels != 1);
	SupportRenderTarget(resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
	SupportUnorderedAccess(resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
}
