#pragma once
#include <pch.h>

#include <aiva/utils/t_cache_updater.h>

namespace aiva::utils
{
	struct IObjectCacheable
	{
	// ----------------------------------------------------
	// Main

	protected:
		IObjectCacheable(bool const initAsChanged);

	public:
		virtual ~IObjectCacheable();

	// ----------------------------------------------------
	// Cache Updater

	public:
		using CacheUpdaterType = TCacheUpdater<IObjectCacheable>;

		using CacheFlagType = CacheUpdaterType::FlagType;

		using CacheActionType = CacheUpdaterType::ActionType;

	public:
		void FlushChanges();

		CacheActionType& OnMarkAsChanged();

		CacheActionType& OnFlushExecuted();

	protected:
		void MarkAsChanged();

		void ClearChanges();

		CacheActionType& FlushExecutors();

	private:
		std::unique_ptr<CacheUpdaterType> mCacheUpdater{};
	};
}
