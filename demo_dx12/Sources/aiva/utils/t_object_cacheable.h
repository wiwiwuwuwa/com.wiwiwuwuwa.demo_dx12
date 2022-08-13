#pragma once
#include <pch.h>

#include <aiva/utils/e_cache_flags.h>
#include <aiva/utils/t_cache_updater.h>

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
		using ThisType = typename TObjectCacheable<typename TDirtyFlags>;

	// ----------------------------------------------------
	// Cache Updater

	public:
		using CacheUpdaterType = TCacheUpdater<ThisType>;

		using CacheFlagType = typename CacheUpdaterType::FlagType;

		using CacheActionType = typename CacheUpdaterType::ActionType;

	public:
		void FlushChanges(typename CacheFlagType const dirtyFlags = CacheFlagType::All);

		typename CacheActionType& OnMarkAsChanged();

		typename CacheActionType& OnFlushExecuted();

	protected:
		void MarkAsChanged(typename CacheFlagType const dirtyFlags = CacheFlagType::All);

		void ClearChanges(typename CacheFlagType const dirtyFlags = CacheFlagType::All);

		typename CacheActionType& FlushExecutors();

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
	void TObjectCacheable<TDirtyFlags>::FlushChanges(typename CacheFlagType const dirtyFlags)
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		mCacheUpdater->FlushChanges(dirtyFlags);
	}

	template <typename TDirtyFlags>
	typename TObjectCacheable<TDirtyFlags>::CacheActionType& TObjectCacheable<TDirtyFlags>::OnMarkAsChanged()
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		return mCacheUpdater->OnMarkAsChanged();
	}

	template <typename TDirtyFlags>
	typename TObjectCacheable<TDirtyFlags>::CacheActionType& TObjectCacheable<TDirtyFlags>::OnFlushExecuted()
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		return mCacheUpdater->OnFlushExecuted();
	}

	template <typename TDirtyFlags>
	void TObjectCacheable<TDirtyFlags>::MarkAsChanged(typename CacheFlagType const dirtyFlags)
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		mCacheUpdater->MarkAsChanged(dirtyFlags);
	}

	template <typename TDirtyFlags>
	void TObjectCacheable<TDirtyFlags>::ClearChanges(typename CacheFlagType const dirtyFlags)
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		mCacheUpdater->ClearChanges(dirtyFlags);
	}

	template <typename TDirtyFlags>
	typename TObjectCacheable<TDirtyFlags>::CacheActionType& TObjectCacheable<TDirtyFlags>::FlushExecutors()
	{
		Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
		return mCacheUpdater->FlushExecutors();
	}
}
