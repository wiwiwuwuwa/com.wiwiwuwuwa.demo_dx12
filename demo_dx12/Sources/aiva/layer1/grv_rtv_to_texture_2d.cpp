#include <pch.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>

#include <aiva/layer1/a_graphic_resource.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GrvRtvToTexture2D::GrvRtvToTexture2D(EngineType const& engine) : AGraphicResourceView{ engine }
{

}

aiva::layer1::GrvRtvToTexture2D::~GrvRtvToTexture2D()
{

}

std::uint64_t aiva::layer1::GrvRtvToTexture2D::MipLevel() const
{
	return mMipLevel;
}

aiva::layer1::GrvRtvToTexture2D& aiva::layer1::GrvRtvToTexture2D::MipLevel(std::uint64_t const mipLevel)
{
	if (mMipLevel != mipLevel)
	{
		mMipLevel = mipLevel;
		MarkAsChanged();
	}

	return *this;
}

aiva::layer1::EDescriptorHeapType aiva::layer1::GrvRtvToTexture2D::HeapType() const
{
	return EDescriptorHeapType::Rtv;
}

aiva::layer1::EResourceViewType aiva::layer1::GrvRtvToTexture2D::ViewType() const
{
	return EResourceViewType::Rtv;
}

void aiva::layer1::GrvRtvToTexture2D::CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination)
{
	auto const& device = Engine().GraphicHardware().Device();
	winrt::check_bool(device);

	auto const& texture2D = std::dynamic_pointer_cast<GrTexture2D>(InternalResource());
	aiva::utils::Asserts::CheckBool(texture2D, "Graphic resource doesn't support texture 2D");
	aiva::utils::Asserts::CheckBool(texture2D->SupportRenderTarget(), "Graphic resource doesn't support render target");

	auto const& resource = texture2D->InternalResource();
	winrt::check_bool(resource);

	auto const& resourceDesc = resource->GetDesc();

	auto viewDesc = D3D12_RENDER_TARGET_VIEW_DESC{};
	viewDesc.Format = resourceDesc.Format;
	viewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = MipLevel();
	viewDesc.Texture2D.PlaneSlice = 0;

	device->CreateRenderTargetView(resource.get(), &viewDesc, destination);
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::GrvRtvToTexture2D::CreateDirectxBarriers(bool const active)
{
	auto const& resource = InternalResource();
	aiva::utils::Asserts::CheckBool(resource, "Graphic resource is not valid");

	auto const& state = active ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_COMMON;
	if (mDesc)
	return resource->PrepareBarriers(state, MipLevel());
}
