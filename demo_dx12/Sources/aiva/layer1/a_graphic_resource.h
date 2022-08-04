#pragma once
#include <pch.h>

#include <aiva/utils/t_cache_updater.h>

namespace aiva::layer1
{
	struct Engine;
	struct GraphicResourceFactory;
}

namespace aiva::utils
{
	struct ResourceBarrier;
}

namespace aiva::layer1
{
	struct AGraphicResource : private boost::noncopyable, public std::enable_shared_from_this<AGraphicResource>
	{
	// ----------------------------------------------------
	// Main

	protected:
		AGraphicResource(aiva::layer1::Engine const& engine);

	public:
		virtual ~AGraphicResource();

	// ----------------------------------------------------
	// Engine

	public:
		aiva::layer1::Engine const& Engine() const;

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Updater

	private:
		using CacheUpdaterType = aiva::utils::TCacheUpdater<AGraphicResource>;

	public:
		CacheUpdaterType::ActionType& OnMarkAsChanged();

	protected:
		void MarkAsChanged();

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};

	// ----------------------------------------------------
	// Internal Resource

	public:
		winrt::com_ptr<ID3D12Resource> const& InternalResource();

	protected:
		virtual void RefreshInternalResource(winrt::com_ptr<ID3D12Resource>& resource, aiva::utils::ResourceBarrier& barrier) = 0;

	private:
		void InitializeInternalResource();

		void TerminateInternalResource();

	private:
		void ExecuteInternalResourceFlush();

	private:
		winrt::com_ptr<ID3D12Resource> mInternalResource{};

	// ----------------------------------------------------
	// Resource Barriers

	public:
		std::vector<D3D12_RESOURCE_BARRIER> PrepareBarriers(D3D12_RESOURCE_STATES const desiredState, std::optional<std::size_t> const subresource = {});

	private:
		void InitializeResourceBarrier();

		void TerminateResourceBarrier();

	private:
		std::unique_ptr<aiva::utils::ResourceBarrier> mResourceBarrier{};
	};
}
