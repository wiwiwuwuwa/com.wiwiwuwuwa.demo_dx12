#include <pch.h>
#include <aiva/layer1/grv_srv_to_texture_2d.h>

//#include <aiva/layer1/engine.h>
//#include <aiva/layer1/gr_texture_2d.h>
//#include <aiva/utils/asserts.h>
//
//aiva::layer1::GrvSrvToTexture2D::GrvSrvToTexture2D(Engine const& engine, GrvSrvToTexture2DDesc const& desc) : mEngine{ engine }
//{
//	Desc(desc);
//}
//
//aiva::layer1::GrvSrvToTexture2D::~GrvSrvToTexture2D()
//{
//	Desc({});
//}
//
//aiva::layer1::EDescriptorHeapType aiva::layer1::GrvSrvToTexture2D::DescriptorHeapType() const
//{
//	return EDescriptorHeapType::CbvSrvUav;
//}
//
//aiva::layer1::EResourceViewType aiva::layer1::GrvSrvToTexture2D::ResourceViewType() const
//{
//	return EResourceViewType::Srv;
//}
//
//aiva::layer1::GrvSrvToTexture2DDesc const& aiva::layer1::GrvSrvToTexture2D::Desc() const
//{
//	return mDesc;
//}
//
//aiva::layer1::GrvSrvToTexture2D& aiva::layer1::GrvSrvToTexture2D::Desc(GrvSrvToTexture2DDesc const& desc)
//{
//	if (mDesc.Resource)
//	{
//		mDesc.Resource->OnInternalResourceUpdated().disconnect(boost::bind(&GrvSrvToTexture2D::OnDescInternalResourceUpdated, this));
//	}
//
//	mDesc = desc;
//
//	if (mDesc.Resource)
//	{
//		mDesc.Resource->OnInternalResourceUpdated().connect(boost::bind(&GrvSrvToTexture2D::OnDescInternalResourceUpdated, this));
//	}
//
//	OnDescInternalResourceUpdated();
//	return *this;
//}
//
//void aiva::layer1::GrvSrvToTexture2D::OnDescInternalResourceUpdated()
//{
//	NotifyInternalResourceUpdated();
//}
//
//std::optional<D3D12_SHADER_RESOURCE_VIEW_DESC> aiva::layer1::GrvSrvToTexture2D::InternalResource()
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
//	
//	auto const& directxResource = aivaResource->InternalResource();
//	winrt::check_bool(directxResource);
//
//	auto const& directxResourceDesc = directxResource->GetDesc();
//
//	D3D12_SHADER_RESOURCE_VIEW_DESC directxViewDesc{};
//	directxViewDesc.Format = directxResourceDesc.Format;
//	directxViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	directxViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	directxViewDesc.Texture2D.MostDetailedMip = 0;
//	directxViewDesc.Texture2D.MipLevels = -1;
//	directxViewDesc.Texture2D.PlaneSlice = 0;
//	directxViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
//
//	return directxViewDesc;
//}
//
//aiva::utils::EvAction& aiva::layer1::GrvSrvToTexture2D::OnInternalResourceUpdated()
//{
//	return mOnInternalResourceUpdated;
//}
//
//void aiva::layer1::GrvSrvToTexture2D::NotifyInternalResourceUpdated()
//{
//	OnInternalResourceUpdated()();
//}
