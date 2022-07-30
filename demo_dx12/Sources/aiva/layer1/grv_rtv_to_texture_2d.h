#pragma once
#include <pch.h>

#include <aiva/layer1/grv_rtv_to_texture_2d_desc.h>
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
	struct GrvRtvToTexture2D final : private boost::noncopyable, public std::enable_shared_from_this<GrvRtvToTexture2D>, public IGpuResourceView
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GrvRtvToTexture2D> Create(TArgs&&... args);

	private:
		GrvRtvToTexture2D(Engine const& engine);

		GrvRtvToTexture2D(Engine const& engine, GrvRtvToTexture2DDesc const& desc);

		GrvRtvToTexture2D(Engine const& engine, std::shared_ptr<GrTexture2D> const& resource);

		GrvRtvToTexture2D(Engine const& engine, winrt::com_ptr<ID3D12Resource> const& resource);

	public:
		~GrvRtvToTexture2D();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		using CacheUpdaterType = aiva::utils::TCacheUpdater<GrvRtvToTexture2D>;

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
		std::optional<GrvRtvToTexture2DDesc> const& Desc() const;

		GrvRtvToTexture2D& Desc(std::optional<GrvRtvToTexture2DDesc> const& desc);

	private:
		void OnDescResourceMarkedAsChanged() const;

	private:
		std::optional<GrvRtvToTexture2DDesc> mDesc{};

	// ----------------------------------------------------
	// Internal Data

	public:
		std::optional<D3D12_RENDER_TARGET_VIEW_DESC> InternalResource() const;
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrvRtvToTexture2D> aiva::layer1::GrvRtvToTexture2D::Create(TArgs&&... args)
{
	return std::shared_ptr<GrvRtvToTexture2D>{new GrvRtvToTexture2D{ std::forward<TArgs>(args)... }};
}
