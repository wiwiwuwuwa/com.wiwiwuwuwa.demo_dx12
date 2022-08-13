#include <pch.h>
#include <aiva/layer1/grv_dsv_to_texture_2d.h>

#include <aiva/layer1/a_graphic_resource.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/object_utils.h>

aiva::layer1::GrvDsvToTexture2D::GrvDsvToTexture2D(EngineType const& engine) : AGraphicResourceView{ engine }
{

}

aiva::layer1::GrvDsvToTexture2D::~GrvDsvToTexture2D()
{

}

std::uint64_t aiva::layer1::GrvDsvToTexture2D::MipLevel() const
{
	return mMipLevel;
}

aiva::layer1::GrvDsvToTexture2D& aiva::layer1::GrvDsvToTexture2D::MipLevel(std::uint64_t const mipLevel)
{
	if (mMipLevel != mipLevel)
	{
		mMipLevel = mipLevel;
		MarkCacheDataAsChanged(EGrvCacheFlags::BufferBin);
	}

	return *this;
}

std::shared_ptr<aiva::layer1::GrvDsvToTexture2D::ResourceType> aiva::layer1::GrvDsvToTexture2D::CreateDefaultInternalResource() const
{
	return aiva::utils::NewObject<GrTexture2D>(Engine());
}

aiva::layer1::EDescriptorHeapType aiva::layer1::GrvDsvToTexture2D::HeapType() const
{
	return EDescriptorHeapType::Dsv;
}

aiva::layer1::EResourceViewType aiva::layer1::GrvDsvToTexture2D::ViewType() const
{
	return EResourceViewType::Dsv;
}

void aiva::layer1::GrvDsvToTexture2D::CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination)
{
	auto const& device = Engine().GraphicHardware().Device();
	winrt::check_bool(device);

	auto const& texture2D = std::dynamic_pointer_cast<GrTexture2D>(GetInternalResource());
	aiva::utils::Asserts::CheckBool(texture2D, "Graphic resource doesn't support texture 2D");
	aiva::utils::Asserts::CheckBool(texture2D->SupportDepthStencil(), "Graphic resource doesn't support depth stencil");

	auto const& resource = texture2D->InternalResource();
	winrt::check_bool(resource);

	auto const& resourceDesc = resource->GetDesc();

	auto viewDesc = D3D12_DEPTH_STENCIL_VIEW_DESC{};
	viewDesc.Format = resourceDesc.Format;
	viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Flags = D3D12_DSV_FLAG_NONE;
	viewDesc.Texture2D.MipSlice = MipLevel();

	device->CreateDepthStencilView(resource.get(), &viewDesc, destination);
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::GrvDsvToTexture2D::CreateDirectxBarriers(bool const active)
{
	auto const& resource = GetInternalResource();
	aiva::utils::Asserts::CheckBool(resource, "Graphic resource is not valid");

	auto const& state = active ? D3D12_RESOURCE_STATE_DEPTH_WRITE : D3D12_RESOURCE_STATE_COMMON;
	return resource->PrepareBarriers(state, MipLevel());
}
