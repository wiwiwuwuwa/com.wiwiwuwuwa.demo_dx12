#pragma once
#include <pch.h>

//#include <aiva/layer1/grv_uav_to_texture_2d_desc.h>
//#include <aiva/layer1/i_gpu_resource_view.h>
//#include <aiva/utils/ev_action.h>
//
//namespace aiva::layer1
//{
//	struct Engine;
//}
//
//namespace aiva::layer1
//{
//	struct GrvUavToTexture2D final : private boost::noncopyable, public std::enable_shared_from_this<GrvUavToTexture2D>, public IGpuResourceView
//	{
//	// ----------------------------------------------------
//	// Main
//
//	public:
//		template <typename... TArgs>
//		static std::shared_ptr<GrvUavToTexture2D> Create(TArgs&&... args);
//
//	private:
//		GrvUavToTexture2D(Engine const& engine, GrvUavToTexture2DDesc const& desc);
//
//	public:
//		~GrvUavToTexture2D();
//
//	private:
//		Engine const& mEngine;
//
//	// ----------------------------------------------------
//	// IGpuResourceView
//
//	public:
//		EGpuDescriptorHeapType DescriptorHeapType() const override;
//
//		EGpuResourceViewType ResourceViewType() const override;
//
//		aiva::utils::EvAction& OnInternalResourceUpdated() override;
//
//	// ----------------------------------------------------
//	// High-Level Data
//
//	public:
//		GrvUavToTexture2DDesc const& Desc() const;
//
//		GrvUavToTexture2D& Desc(GrvUavToTexture2DDesc const& desc);
//
//	private:
//		void OnDescInternalResourceUpdated();
//
//	private:
//		GrvUavToTexture2DDesc mDesc{};
//
//	// ----------------------------------------------------
//	// Low-Level Data
//
//	public:
//		std::optional<D3D12_UNORDERED_ACCESS_VIEW_DESC> InternalResource();
//
//	private:
//		void NotifyInternalResourceUpdated();
//
//	private:
//		aiva::utils::EvAction mOnInternalResourceUpdated{};
//	};
//}
//
//// --------------------------------------------------------
//
//template <typename... TArgs>
//std::shared_ptr<aiva::layer1::GrvUavToTexture2D> aiva::layer1::GrvUavToTexture2D::Create(TArgs&&... args)
//{
//	return std::shared_ptr<GrvUavToTexture2D>{new GrvUavToTexture2D{ std::forward<TArgs>(args)... }};
//}
