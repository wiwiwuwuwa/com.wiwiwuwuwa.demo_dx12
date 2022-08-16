#include <pch.h>
#include <aiva/layer1/gr_texture_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/resource_barrier.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	GrTexture2D::GrTexture2D(EngineType const& engine) : AGraphicResource(engine)
	{

	}

	GrTexture2D::~GrTexture2D()
	{

	}

	EResourceBufferFormat GrTexture2D::Format() const
	{
		return mFormat;
	}

	GrTexture2D& GrTexture2D::Format(EResourceBufferFormat const format)
	{
		if (mFormat != format)
		{
			mFormat = format;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	std::size_t GrTexture2D::Width() const
	{
		return mWidth;
	}

	GrTexture2D& GrTexture2D::Width(std::size_t const width)
	{
		if (mWidth != width)
		{
			mWidth = width;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	std::size_t GrTexture2D::Height() const
	{
		return mHeight;
	}

	GrTexture2D& GrTexture2D::Height(std::size_t const height)
	{
		if (mHeight != height)
		{
			mHeight = height;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	bool GrTexture2D::SupportDepthStencil() const
	{
		return mSupportDepthStencil;
	}

	GrTexture2D& GrTexture2D::SupportDepthStencil(bool const support)
	{
		if (mSupportDepthStencil != support)
		{
			mSupportDepthStencil = support;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	bool GrTexture2D::SupportMipMapping() const
	{
		return mSupportMipMapping;
	}

	GrTexture2D& GrTexture2D::SupportMipMapping(bool const support)
	{
		if (mSupportMipMapping != support)
		{
			mSupportMipMapping = support;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	bool GrTexture2D::SupportRenderTarget() const
	{
		return mSupportRenderTarget;
	}

	GrTexture2D& GrTexture2D::SupportRenderTarget(bool const support)
	{
		if (mSupportRenderTarget != support)
		{
			mSupportRenderTarget = support;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	bool GrTexture2D::SupportShaderAtomics() const
	{
		return mSupportShaderAtomics;
	}

	GrTexture2D& GrTexture2D::SupportShaderAtomics(bool const support)
	{
		if (mSupportShaderAtomics != support)
		{
			mSupportShaderAtomics = support;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	bool GrTexture2D::SupportUnorderedAccess() const
	{
		return mSupportUnorderedAccess;
	}

	GrTexture2D& GrTexture2D::SupportUnorderedAccess(bool const support)
	{
		if (mSupportUnorderedAccess != support)
		{
			mSupportUnorderedAccess = support;
			MarkCacheDataAsChanged();
		}

		return *this;
	}

	void GrTexture2D::RefreshInternalResourceFromSelf(ResourceType& resource, BarrierType& barrier)
	{
		Asserts::CheckBool(Width() > 0, "Width is not valid");
		Asserts::CheckBool(Height() > 0, "Height is not valid");
		Asserts::CheckBool(SupportDepthStencil() == SupportsDepthStencil(Format()), "Format doesn't support depth stencil");
		Asserts::CheckBool(!(SupportMipMapping() && Width() != Height()), "Non-square texture doesn't support mip maps");
		Asserts::CheckBool(SupportRenderTarget() != SupportsDepthStencil(Format()), "Format doesn't support render target");
		Asserts::CheckBool(!(SupportRenderTarget() && SupportDepthStencil()), "Format doesn't support render target and depth stencil at same time");
		Asserts::CheckBool(!(SupportDepthStencil() && SupportUnorderedAccess()), "Format doesn't support depth stencil and unordered access at same time");

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

		auto commitedResource = ResourceType{};
		winrt::check_hresult(device->CreateCommittedResource(&heapProperties, heapFlags, &resourceDesc, resourceStates, nullptr, IID_PPV_ARGS(&commitedResource)));

		winrt::check_bool(commitedResource);
		resource = commitedResource;
	}

	void GrTexture2D::RefreshSelfFromInternalResource(ResourceType const& resource)
	{
		winrt::check_bool(resource);

		auto heapProperties = D3D12_HEAP_PROPERTIES{};
		auto heapFlags = D3D12_HEAP_FLAGS{};
		winrt::check_hresult(resource->GetHeapProperties(&heapProperties, &heapFlags));

		SupportShaderAtomics(heapFlags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS);

		auto const& resourceDesc = resource->GetDesc();
		Asserts::CheckBool(resourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D, "Resource is not a texture 2D");

		Format(FromInternalEnum(resourceDesc.Format));
		Width(resourceDesc.Width);
		Height(resourceDesc.Height);
		SupportDepthStencil(resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		SupportMipMapping(resourceDesc.MipLevels != 1);
		SupportRenderTarget(resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		SupportUnorderedAccess(resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	}

}
