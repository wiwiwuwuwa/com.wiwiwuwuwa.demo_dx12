#include <pch.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>

#include <aiva/layer1/a_graphic_resource.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/object_utils.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	GrvRtvToTexture2D::GrvRtvToTexture2D(EngineType const& engine) : AGraphicResourceView{ engine }
	{

	}

	GrvRtvToTexture2D::~GrvRtvToTexture2D()
	{

	}

	std::size_t GrvRtvToTexture2D::MipLevel() const
	{
		return mMipLevel;
	}

	GrvRtvToTexture2D::ThisType& GrvRtvToTexture2D::MipLevel(std::size_t const mipLevel)
	{
		if (mMipLevel != mipLevel)
		{
			mMipLevel = mipLevel;
			MarkCacheDataAsChanged(CacheFlagType::BufferBin);
		}

		return *this;
	}

	GrvRtvToTexture2D::ParentType::ResourceTypeShared GrvRtvToTexture2D::CreateDefaultInternalResource() const
	{
		auto const resource = NewObject<ResourceType>(Engine());
		Asserts::CheckBool(resource, "Resource is not valid");

		resource->Format(EResourceBufferFormat::R32G32B32A32_FLOAT);
		resource->SupportRenderTarget(true);
		return resource;
	}

	GrvRtvToTexture2D::HeapTypeEnum GrvRtvToTexture2D::HeapType() const
	{
		return HeapTypeEnum::Rtv;
	}

	GrvRtvToTexture2D::ViewTypeEnum GrvRtvToTexture2D::ViewType() const
	{
		return ViewTypeEnum::Rtv;
	}

	void GrvRtvToTexture2D::CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination)
	{
		auto const& device = Engine().GraphicHardware().Device();
		winrt::check_bool(device);

		auto const texture2D = std::dynamic_pointer_cast<GrTexture2D>(GetInternalResource());
		Asserts::CheckBool(texture2D, "Graphic resource doesn't support texture 2D");
		Asserts::CheckBool(texture2D->SupportRenderTarget(), "Graphic resource doesn't support render target");

		auto const& resource = texture2D->GetInternalResource();
		winrt::check_bool(resource);

		auto const resourceDesc = resource->GetDesc();

		auto viewDesc = D3D12_RENDER_TARGET_VIEW_DESC{};
		viewDesc.Format = resourceDesc.Format;
		viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice = MipLevel();
		viewDesc.Texture2D.PlaneSlice = 0;

		device->CreateRenderTargetView(resource.get(), &viewDesc, destination);
	}

	std::vector<D3D12_RESOURCE_BARRIER> GrvRtvToTexture2D::CreateDirectxBarriers(bool const active)
	{
		auto const& resource = GetInternalResource();
		Asserts::CheckBool(resource, "Graphic resource is not valid");

		auto const state = active ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_COMMON;
		return resource->CreateDirectxBarriers(state, MipLevel());
	}
}
