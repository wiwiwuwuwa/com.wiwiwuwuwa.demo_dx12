#pragma once
#include <pch.h>

#include <aiva/layer1/grv_srv_to_texture_2d_desc.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GrvSrvToTexture2D final : private boost::noncopyable, public std::enable_shared_from_this<GrvSrvToTexture2D>, public IGpuResourceView
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GrvSrvToTexture2D> Create(TArgs&&... args);

	private:
		GrvSrvToTexture2D(Engine const& engine, GrvSrvToTexture2DDesc const& desc);

	public:
		~GrvSrvToTexture2D();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// High-Level Data

	public:
		GrvSrvToTexture2DDesc const& Desc() const;

		GrvSrvToTexture2D& Desc(GrvSrvToTexture2DDesc const& desc);

	private:
		void OnDescInternalResourceUpdated();

	private:
		GrvSrvToTexture2DDesc mDesc{};

	// ----------------------------------------------------
	// Low-Level Data

	public:
		std::optional<D3D12_SHADER_RESOURCE_VIEW_DESC> InternalResource();

		aiva::utils::EvAction& OnInternalResourceUpdated();

	private:
		void NotifyInternalResourceUpdated();

	private:
		aiva::utils::EvAction mOnInternalResourceUpdated{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrvSrvToTexture2D> aiva::layer1::GrvSrvToTexture2D::Create(TArgs&&... args)
{
	return std::shared_ptr<GrvSrvToTexture2D>{new GrvSrvToTexture2D{ std::forward<TArgs>(args)... }};
}
