#pragma once
#include <pch.h>

#include <aiva/layer1/i_gpu_resource.h>
#include <aiva/layer1/gr_texture_2d_desc.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GrTexture2D final : private boost::noncopyable, public std::enable_shared_from_this<GrTexture2D>, public aiva::layer1::IGpuResource
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GrTexture2D> Create(TArgs&&... args);

	private:
		GrTexture2D(Engine const& engine, GrTexture2DDesc const& desc);

	public:
		~GrTexture2D();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// High-Level Data

	public:
		GrTexture2DDesc Desc() const;

		GrTexture2D& Desc(GrTexture2DDesc const& desc);

	// ----------------------------------------------------
	// Low-Level Data

	public:
		winrt::com_ptr<ID3D12Resource> const& InternalResource();

		aiva::utils::EvAction& OnInternalResourceUpdated();

	private:
		void UpdateInternalResource(GrTexture2DDesc const& desc);

	private:
		winrt::com_ptr<ID3D12Resource> mInternalResource{};

		aiva::utils::EvAction mOnInternalResourceUpdated{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrTexture2D> aiva::layer1::GrTexture2D::Create(TArgs&&... args)
{
	return std::shared_ptr<GrTexture2D>{new GrTexture2D{ std::forward<TArgs>(args)... }};
}
