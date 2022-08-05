#include <pch.h>
#include <aiva/layer1/a_graphic_object.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>

aiva::layer1::AGraphicObject::AGraphicObject(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
}

aiva::layer1::AGraphicObject::~AGraphicObject()
{
	TerminateCacheUpdater();
}

aiva::layer1::Engine const& aiva::layer1::AGraphicObject::Engine() const
{
	return mEngine;
}

void aiva::layer1::AGraphicObject::FlushChanges()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	mCacheUpdater->FlushChanges();
}

aiva::layer1::AGraphicObject::CacheUpdaterType::ActionType& aiva::layer1::AGraphicObject::OnMarkAsChanged()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	return mCacheUpdater->OnMarkAsChanged();
}

aiva::layer1::AGraphicObject::CacheUpdaterType::ActionType& aiva::layer1::AGraphicObject::OnFlushExecuted()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	return mCacheUpdater->OnFlushExecuted();
}

void aiva::layer1::AGraphicObject::MarkAsChanged()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	mCacheUpdater->MarkAsChanged();
}

void aiva::layer1::AGraphicObject::ClearChanges()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	mCacheUpdater->ClearChanges();
}

aiva::layer1::AGraphicObject::CacheUpdaterType::ActionType& aiva::layer1::AGraphicObject::FlushExecutors()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	return mCacheUpdater->FlushExecutors();
}

void aiva::layer1::AGraphicObject::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
}

void aiva::layer1::AGraphicObject::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	mCacheUpdater = {};
}
