#include <pch.h>
#include <aiva/layer1/grv_dsv_to_texture_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/utils/asserts.h>

aiva::layer1::GrvDsvToTexture2D::GrvDsvToTexture2D(Engine const& engine, GrvDsvToTexture2DDesc const& desc) : mEngine{ engine }
{
	Desc(desc);
}

aiva::layer1::GrvDsvToTexture2D::~GrvDsvToTexture2D()
{
	Desc({});
}

aiva::layer1::GrvDsvToTexture2DDesc const& aiva::layer1::GrvDsvToTexture2D::Desc() const
{
	return mDesc;
}

aiva::layer1::GrvDsvToTexture2D& aiva::layer1::GrvDsvToTexture2D::Desc(GrvDsvToTexture2DDesc const& desc)
{
	if (mDesc.Resource)
	{
		mDesc.Resource->OnInternalResourceUpdated().disconnect(boost::bind(&GrvDsvToTexture2D::OnDescInternalResourceUpdated, this));
	}

	mDesc = desc;

	if (mDesc.Resource)
	{
		mDesc.Resource->OnInternalResourceUpdated().connect(boost::bind(&GrvDsvToTexture2D::OnDescInternalResourceUpdated, this));
	}

	OnDescInternalResourceUpdated();
	return *this;
}

void aiva::layer1::GrvDsvToTexture2D::OnDescInternalResourceUpdated()
{
	NotifyInternalResourceUpdated();
}

std::optional<D3D12_DEPTH_STENCIL_VIEW_DESC> aiva::layer1::GrvDsvToTexture2D::InternalResource()
{
	auto const& aivaViewDesc = Desc();
	if (!aivaViewDesc.Resource)
	{
		return {};
	}

	auto const& aivaResource = aivaViewDesc.Resource;
	aiva::utils::Asserts::CheckBool(aivaResource);

	auto const& aivaResourceDesc = aivaResource->Desc();
	aiva::utils::Asserts::CheckBool(aivaResourceDesc.SupportDepthStencil);

	auto const& directxResource = aivaResource->InternalResource();
	winrt::check_bool(directxResource);

	auto const& directxResourceDesc = directxResource->GetDesc();

	D3D12_DEPTH_STENCIL_VIEW_DESC directxViewDesc{};
	directxViewDesc.Format = directxResourceDesc.Format;
	directxViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	directxViewDesc.Flags = D3D12_DSV_FLAG_NONE;
	directxViewDesc.Texture2D.MipSlice = aivaViewDesc.MipLevel;

	return directxViewDesc;
}

aiva::utils::EvAction& aiva::layer1::GrvDsvToTexture2D::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}

void aiva::layer1::GrvDsvToTexture2D::NotifyInternalResourceUpdated()
{
	OnInternalResourceUpdated()();
}
