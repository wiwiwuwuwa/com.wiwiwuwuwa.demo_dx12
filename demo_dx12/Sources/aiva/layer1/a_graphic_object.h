#pragma once
#include <pch.h>

#include <aiva/utils/t_cache_updater.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct AGraphicObject : private boost::noncopyable, public std::enable_shared_from_this<AGraphicObject>
	{
	// ----------------------------------------------------
	// Main

	protected:
		AGraphicObject(aiva::layer1::Engine const& engine);

	public:
		virtual ~AGraphicObject();

	// ----------------------------------------------------
	// Engine

	public:
		aiva::layer1::Engine const& Engine() const;

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// Cache Updater

	public:
		using CacheUpdaterType = aiva::utils::TCacheUpdater<AGraphicObject>;

	public:
		void FlushChanges();

		CacheUpdaterType::ActionType& OnMarkAsChanged();

		CacheUpdaterType::ActionType& OnFlushExecuted();

	protected:
		void MarkAsChanged();

		void ClearChanges();

		CacheUpdaterType::ActionType& FlushExecutors();

	private:
		void InitializeCacheUpdater();

		void TerminateCacheUpdater();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};
	};
}