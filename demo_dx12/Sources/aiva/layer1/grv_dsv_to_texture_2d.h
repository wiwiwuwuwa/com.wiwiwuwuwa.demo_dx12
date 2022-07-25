#pragma once
#include <pch.h>

#include <aiva/layer1/grv_dsv_to_texture_2d_desc.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GrvDsvToTexture2D final : private boost::noncopyable, public std::enable_shared_from_this<GrvDsvToTexture2D>, public IGpuResourceView
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GrvDsvToTexture2D> Create(TArgs&&... args);

	private:
		GrvDsvToTexture2D(Engine const& engine, GrvDsvToTexture2DDesc const& desc);

	public:
		~GrvDsvToTexture2D();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// IGpuResourceView

	public:
		EGpuDescriptorHeapType DescriptorHeapType() const override;

		aiva::utils::EvAction& OnInternalResourceUpdated() override;

	// ----------------------------------------------------
	// High-Level Data

	public:
		GrvDsvToTexture2DDesc const& Desc() const;

		GrvDsvToTexture2D& Desc(GrvDsvToTexture2DDesc const& desc);

	private:
		void OnDescInternalResourceUpdated();

	private:
		GrvDsvToTexture2DDesc mDesc{};

	// ----------------------------------------------------
	// Low-Level Data

	public:
		std::optional<D3D12_DEPTH_STENCIL_VIEW_DESC> InternalResource();

	private:
		void NotifyInternalResourceUpdated();

	private:
		aiva::utils::EvAction mOnInternalResourceUpdated{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrvDsvToTexture2D> aiva::layer1::GrvDsvToTexture2D::Create(TArgs&&... args)
{
	return std::shared_ptr<GrvDsvToTexture2D>{new GrvDsvToTexture2D{ std::forward<TArgs>(args)... }};
}
