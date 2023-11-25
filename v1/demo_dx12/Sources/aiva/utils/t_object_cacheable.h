#pragma once
#include <pch.h>

#include <aiva/utils/e_cache_flags.h>
#include <aiva/utils/t_cache_updater.h>
#include <aiva/utils/t_object_changeable.h>

namespace aiva::utils
{
	template <typename TDirtyFlags = ECacheFlags>
	struct TObjectCacheable
	{
	// ----------------------------------------------------
	// Main

	protected:
		TObjectCacheable(bool const initAsChanged);

	public:
		virtual ~TObjectCacheable();

	// ----------------------------------------------------
	// Aliases

	public:
		using FlagType = TDirtyFlags;

		using ThisType = typename TObjectCacheable<typename FlagType>;

	// ----------------------------------------------------
	// Cache Updater

	public:
		using CacheUpdaterType = TCacheUpdater<ThisType, FlagType>;

		using CacheFlagType = typename CacheUpdaterType::FlagType;

		using CacheActionType = typename CacheUpdaterType::ActionType;

	// --------------------------------

	public:
		void FlushCacheDataChanges(typename CacheFlagType const dirtyFlags = CacheFlagType::All);

		typename CacheActionType& OnMarkCacheDataAsChanged();

		typename CacheActionType& OnFlushCacheDataExecuted();

	protected:
		void MarkCacheDataAsChanged(typename CacheFlagType const dirtyFlags = CacheFlagType::All);

		void SkipCacheDataChanges(typename CacheFlagType const dirtyFlags = CacheFlagType::All);

		typename CacheActionType& FlushCacheDataExecutors();

	private:
		std::unique_ptr<typename CacheUpdaterType> mCacheUpdater{};
	};
}

// --------------------------------------------------------

#include <aiva/utils/asserts.h>

namespace aiva::utils
{
	template <typename TDirtyFlags>
	TObjectCacheable<TDirtyFlags>::TObjectCacheable(bool const initAsChanged)
	{
		auto const dirtyFlags = initAsChanged ? CacheFlagType::All : CacheFlagType::None;

		mCacheUpdater = std::make_unique<CacheUpdaterType>(dirtyFlags);
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	}

	template <typename TDirtyFlags>
	TObjectCacheable<TDirtyFlags>::~TObjectCacheable()
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		mCacheUpdater = {};
	}

	template <typename TDirtyFlags>
	void TObjectCacheable<TDirtyFlags>::FlushCacheDataChanges(typename CacheFlagType const dirtyFlags)
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		mCacheUpdater->FlushChanges(dirtyFlags);
	}

	template <typename TDirtyFlags>
	typename TObjectCacheable<TDirtyFlags>::CacheActionType& TObjectCacheable<TDirtyFlags>::OnMarkCacheDataAsChanged()
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		return mCacheUpdater->OnMarkAsChanged();
	}

	template <typename TDirtyFlags>
	typename TObjectCacheable<TDirtyFlags>::CacheActionType& TObjectCacheable<TDirtyFlags>::OnFlushCacheDataExecuted()
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		return mCacheUpdater->OnFlushExecuted();
	}

	template <typename TDirtyFlags>
	void TObjectCacheable<TDirtyFlags>::MarkCacheDataAsChanged(typename CacheFlagType const dirtyFlags)
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		mCacheUpdater->MarkAsChanged(dirtyFlags);
	}

	template <typename TDirtyFlags>
	void TObjectCacheable<TDirtyFlags>::SkipCacheDataChanges(typename CacheFlagType const dirtyFlags)
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		mCacheUpdater->SkipChanges(dirtyFlags);
	}

	template <typename TDirtyFlags>
	typename TObjectCacheable<TDirtyFlags>::CacheActionType& TObjectCacheable<TDirtyFlags>::FlushCacheDataExecutors()
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		return mCacheUpdater->FlushExecutors();
	}
}
