#include <pch.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GrvRtvToTexture2D::GrvRtvToTexture2D(Engine const& engine, GrvRtvToTexture2DDesc const& desc) : mEngine{ engine }
{
	Desc(desc);
}

aiva::layer1::GrvRtvToTexture2D::~GrvRtvToTexture2D()
{
	Desc({});
}

aiva::layer1::EGpuDescriptorHeapType aiva::layer1::GrvRtvToTexture2D::DescriptorHeapType() const
{
	return EGpuDescriptorHeapType::Rtv;
}

aiva::layer1::EGpuResourceViewType aiva::layer1::GrvRtvToTexture2D::ResourceViewType() const
{
	return EGpuResourceViewType::Rtv;
}

aiva::layer1::GrvRtvToTexture2DDesc const& aiva::layer1::GrvRtvToTexture2D::Desc() const
{
	return mDesc;
}

aiva::layer1::GrvRtvToTexture2D& aiva::layer1::GrvRtvToTexture2D::Desc(GrvRtvToTexture2DDesc const& desc)
{
	if (mDesc.Resource)
	{
		mDesc.Resource->OnInternalResourceUpdated().disconnect(boost::bind(&GrvRtvToTexture2D::OnDescInternalResourceUpdated, this));
	}

	mDesc = desc;

	if (mDesc.Resource)
	{
		mDesc.Resource->OnInternalResourceUpdated().connect(boost::bind(&GrvRtvToTexture2D::OnDescInternalResourceUpdated, this));
	}

	OnDescInternalResourceUpdated();
	return *this;
}

void aiva::layer1::GrvRtvToTexture2D::OnDescInternalResourceUpdated()
{
	NotifyInternalResourceUpdated();
}

std::optional<D3D12_RENDER_TARGET_VIEW_DESC> aiva::layer1::GrvRtvToTexture2D::InternalResource()
{
	auto const& aivaViewDesc = Desc();
	if (!aivaViewDesc.Resource)
	{
		return {};
	}

	auto const& aivaResource = aivaViewDesc.Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);

	auto const& aivaResourceDesc = aivaResource->Desc();
	aiva::utils::Asserts::CheckBool(aivaResourceDesc.SupportRenderTarget);

	auto const& directxResource = aivaResource->InternalResource();
	winrt::check_bool(directxResource);

	auto const& directxResourceDesc = directxResource->GetDesc();

	D3D12_RENDER_TARGET_VIEW_DESC directxViewDesc{};
	directxViewDesc.Format = directxResourceDesc.Format;
	directxViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	directxViewDesc.Texture2D.MipSlice = aivaViewDesc.MipLevel;
	directxViewDesc.Texture2D.PlaneSlice = 0;

	return directxViewDesc;
}

aiva::utils::EvAction& aiva::layer1::GrvRtvToTexture2D::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}

void aiva::layer1::GrvRtvToTexture2D::NotifyInternalResourceUpdated()
{
	OnInternalResourceUpdated()();
}
