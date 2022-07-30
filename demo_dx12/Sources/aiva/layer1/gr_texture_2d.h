#pragma once
#include <pch.h>

#include <aiva/layer1/i_gpu_resource.h>
#include <aiva/layer1/gr_texture_2d_desc.h>
#include <aiva/utils/ev_action.h>
#include <aiva/utils/resource_barrier.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::utils
{
	template <typename, typename>
	struct TCacheUpdater;
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
		GrTexture2D(Engine const& engine);

		GrTexture2D(Engine const& engine, GrTexture2DDesc const& desc);

		GrTexture2D(Engine const& engine, winrt::com_ptr<ID3D12Resource> const& resource);

	public:
		~GrTexture2D();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Refresh

	public:
		enum class EDirtyFlags
		{
			None = 0,
			All = 1,
		};

		using CacheUpdaterType = aiva::utils::TCacheUpdater<GrTexture2D, EDirtyFlags>;

	public:
		CacheUpdaterType& CacheUpdater() const;

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// ----------------------------------------------------
	// High-Level Data

	public:
		std::optional<GrTexture2DDesc> const& Desc() const;

		GrTexture2D& Desc(std::optional<GrTexture2DDesc> const& desc);

	private:
		std::optional<GrTexture2DDesc> mDesc{};

	// ----------------------------------------------------
	// Low-Level Data

	public:
		winrt::com_ptr<ID3D12Resource> const InternalResource();

	private:
		GrTexture2D& InternalResource(winrt::com_ptr<ID3D12Resource> const& resource);

	private:
		void InitializeInternalResources();

		void TerminateInternalResources();

	private:
		void RefreshInternalResources();

		static winrt::com_ptr<ID3D12Resource> CreateInternalResource(Engine const& engine, GrTexture2DDesc const& desc, aiva::utils::ResourceBarrier& outBarrier);

	private:
		winrt::com_ptr<ID3D12Resource> mInternalResource{};

	// ----------------------------------------------------
	// Resource Barriers

	public:
		std::vector<D3D12_RESOURCE_BARRIER> PrepareBarriers(D3D12_RESOURCE_STATES const desiredState, std::optional<std::size_t> const subresource = {});

	private:
		aiva::utils::ResourceBarrier mResourceBarrier{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GrTexture2D> aiva::layer1::GrTexture2D::Create(TArgs&&... args)
{
	return std::shared_ptr<GrTexture2D>{new GrTexture2D{ std::forward<TArgs>(args)... }};
}
