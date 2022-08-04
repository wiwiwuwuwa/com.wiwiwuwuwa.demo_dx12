#include <pch.h>
#include <aiva/layer1/grv_uav_to_texture_2d.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/utils/asserts.h>

//aiva::layer1::GrvUavToTexture2D::GrvUavToTexture2D(Engine const& engine, GrvUavToTexture2DDesc const& desc) : mEngine{ engine }
//{
//	Desc(desc);
//}
//
//aiva::layer1::GrvUavToTexture2D::~GrvUavToTexture2D()
//{
//	Desc({});
//}
//
//aiva::layer1::EDescriptorHeapType aiva::layer1::GrvUavToTexture2D::DescriptorHeapType() const
//{
//	return EDescriptorHeapType::CbvSrvUav;
//}
//
//aiva::layer1::EResourceViewType aiva::layer1::GrvUavToTexture2D::ResourceViewType() const
//{
//	return EResourceViewType::Uav;
//}
//
//aiva::layer1::GrvUavToTexture2DDesc const& aiva::layer1::GrvUavToTexture2D::Desc() const
//{
//	return mDesc;
//}
//
//aiva::layer1::GrvUavToTexture2D& aiva::layer1::GrvUavToTexture2D::Desc(GrvUavToTexture2DDesc const& desc)
//{
//	if (mDesc.Resource)
//	{
//		mDesc.Resource->OnInternalResourceUpdated().disconnect(boost::bind(&GrvUavToTexture2D::OnDescInternalResourceUpdated, this));
//	}
//
//	mDesc = desc;
//
//	if (mDesc.Resource)
//	{
//		mDesc.Resource->OnInternalResourceUpdated().connect(boost::bind(&GrvUavToTexture2D::OnDescInternalResourceUpdated, this));
//	}
//
//	OnDescInternalResourceUpdated();
//	return *this;
//}
//
//void aiva::layer1::GrvUavToTexture2D::OnDescInternalResourceUpdated()
//{
//	NotifyInternalResourceUpdated();
//}
//
//std::optional<D3D12_UNORDERED_ACCESS_VIEW_DESC> aiva::layer1::GrvUavToTexture2D::InternalResource()
//{
//	auto const& aivaViewDesc = Desc();
//	if (!aivaViewDesc.Resource)
//	{
//		return {};
//	}
//
//	auto const& aivaResource = aivaViewDesc.Resource;
//	aiva::utils::Asserts::CheckBool(aivaResource);
//
//	auto const& aivaResourceDesc = aivaResource->Desc();
//	aiva::utils::Asserts::CheckBool(aivaResourceDesc.SupportUnorderedAccess);
//
//	auto const& directxResource = aivaResource->InternalResource();
//	winrt::check_bool(directxResource);
//
//	auto const& directxResourceDesc = directxResource->GetDesc();
//
//	D3D12_UNORDERED_ACCESS_VIEW_DESC directxViewDesc{};
//	directxViewDesc.Format = directxResourceDesc.Format;
//	directxViewDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
//	directxViewDesc.Texture2D.MipSlice = aivaViewDesc.MipLevel;
//	directxViewDesc.Texture2D.PlaneSlice = 0;
//
//	return directxViewDesc;
//}
//
//aiva::utils::EvAction& aiva::layer1::GrvUavToTexture2D::OnInternalResourceUpdated()
//{
//	return mOnInternalResourceUpdated;
//}
//
//void aiva::layer1::GrvUavToTexture2D::NotifyInternalResourceUpdated()
//{
//	OnInternalResourceUpdated()();
//}
