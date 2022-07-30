#pragma once
#include <pch.h>

#include <aiva/layer1/grv_dsv_to_texture_2d_desc.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	enum class EGpuDescriptorHeapType : std::uint8_t;
	enum class EGpuResourceViewType : std::uint8_t;

	struct Engine;
}

namespace aiva::utils
{
	enum class ECacheFlags : std::uint8_t;

	template <typename, typename = ECacheFlags>
	struct TCacheUpdater;
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
		GrvDsvToTexture2D(Engine const& engine);

	public:
		~GrvDsvToTexture2D();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		using CacheUpdaterType = aiva::utils::TCacheUpdater<GrvDsvToTexture2D>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// ----------------------------------------------------
	// IGpuResourceView

	public:
		EGpuDescriptorHeapType HeapType() const override;

		EGpuResourceViewType ViewType() const override;

		void CreateView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) const override;

		aiva::utils::TEvAction<aiva::utils::ECacheFlags>& OnMarkAsChanged() override;

	// ----------------------------------------------------
	// Desc Data

	public:
		std::optional<GrvDsvToTexture2DDesc> const& Desc() const;

		GrvDsvToTexture2D& Desc(std::optional<GrvDsvToTexture2DDesc> const& desc);

	private:
		void OnDescResourceMarkedAsChanged() const;

	private:
		std::optional<GrvDsvToTexture2DDesc> mDesc{};

	// ----------------------------------------------------
	// Internal Data

	public:
		std::optional<D3D12_DEPTH_STENCIL_VIEW_DESC> InternalResource() const;
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrvDsvToTexture2D> aiva::layer1::GrvDsvToTexture2D::Create(TArgs&&... args)
{
	return std::shared_ptr<GrvDsvToTexture2D>{new GrvDsvToTexture2D{ std::forward<TArgs>(args)... }};
}
