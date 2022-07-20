#pragma once
#include <pch.h>

#include <aiva/layer1/grv_rtv_to_texture_2d_desc.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GrvRtvToTexture2D final : private boost::noncopyable, public std::enable_shared_from_this<GrvRtvToTexture2D>, public IGpuResourceView
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GrvRtvToTexture2D> Create(TArgs&&... args);

	private:
		GrvRtvToTexture2D(Engine const& engine, GrvRtvToTexture2DDesc const& desc);

	public:
		~GrvRtvToTexture2D();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// High-Level Data

	public:
		GrvRtvToTexture2DDesc const& Desc() const;

		GrvRtvToTexture2D& Desc(GrvRtvToTexture2DDesc const& desc);

	private:
		void OnDescInternalResourceUpdated();

	private:
		GrvRtvToTexture2DDesc mDesc{};

	// ----------------------------------------------------
	// Low-Level Data

	public:
		std::optional<D3D12_RENDER_TARGET_VIEW_DESC> InternalResource();
		
		aiva::utils::EvAction& OnInternalResourceUpdated();

	private:
		void NotifyInternalResourceUpdated();

	private:
		aiva::utils::EvAction mOnInternalResourceUpdated{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrvRtvToTexture2D> aiva::layer1::GrvRtvToTexture2D::Create(TArgs&&... args)
{
	return std::shared_ptr<GrvRtvToTexture2D>{new GrvRtvToTexture2D{ std::forward<TArgs>(args)... }};
}
