#include <pch.h>
#include <aiva/utils/i_object_cacheable.h>

aiva::utils::IObjectCacheable::IObjectCacheable(bool const initAsChanged)
{
	auto const dirtyFlags = initAsChanged ? CacheFlagType::All : CacheFlagType::None;

	mCacheUpdater = std::make_unique<CacheUpdaterType>(dirtyFlags);
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
}

aiva::utils::IObjectCacheable::~IObjectCacheable()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	mCacheUpdater = {};
}

void aiva::utils::IObjectCacheable::FlushChanges()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	mCacheUpdater->FlushChanges();
}

aiva::utils::IObjectCacheable::CacheActionType& aiva::utils::IObjectCacheable::OnMarkAsChanged()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	return mCacheUpdater->OnMarkAsChanged();
}

aiva::utils::IObjectCacheable::CacheActionType& aiva::utils::IObjectCacheable::OnFlushExecuted()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	return mCacheUpdater->OnFlushExecuted();
}

void aiva::utils::IObjectCacheable::MarkAsChanged()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	mCacheUpdater->MarkAsChanged();
}

void aiva::utils::IObjectCacheable::ClearChanges()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	mCacheUpdater->ClearChanges();
}

aiva::utils::IObjectCacheable::CacheActionType& aiva::utils::IObjectCacheable::FlushExecutors()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	return mCacheUpdater->FlushExecutors();
}
