#include <pch.h>
#include <aiva/layer1/a_graphic_resource_view.h>

#include <aiva/layer1/a_graphic_resource.h>
#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	AGraphicResourceView::AGraphicResourceView(EngineType const& engine)
		: AObject{}, TObjectCacheable<CacheFlagType>{ true }, IObjectEngineable{ engine }
	{
		InitializeInternalResource();
	}

	AGraphicResourceView::~AGraphicResourceView()
	{
		TerminateInternalResource();
	}

	AGraphicResourceView::ResourceTypeShared const& aiva::layer1::AGraphicResourceView::GetInternalResource()
	{
		FlushCacheDataChanges();
		return mInternalResource;
	}

	AGraphicResourceView::ResourceTypeShared const& AGraphicResourceView::GetOrAddInternalResource()
	{
		auto const& existingInternalResource = GetInternalResource();
		if (existingInternalResource)
		{
			return existingInternalResource;
		}

		auto const& createdInternalResource = SetInternalResource(CreateDefaultInternalResource()).GetInternalResource();
		if (createdInternalResource)
		{
			return createdInternalResource;
		}

		Asserts::CheckBool(false, "Failed to get or add internal resource");
		return {};
	}

	AGraphicResourceView& AGraphicResourceView::SetInternalResource(ResourceTypeShared const& resource)
	{
		if (mInternalResource == resource)
		{
			return *this;
		}

		if (mInternalResource)
		{
			mInternalResource->OnMarkCacheDataAsChanged().disconnect(boost::bind(&ThisType::InternalResource_OnMarkCacheDataAsChanged, this));
		}

		mInternalResource = resource;

		if (mInternalResource)
		{
			mInternalResource->OnMarkCacheDataAsChanged().connect(boost::bind(&ThisType::InternalResource_OnMarkCacheDataAsChanged, this));
		}

		MarkCacheDataAsChanged();
		return *this;
	}

	void AGraphicResourceView::InternalResource_OnMarkCacheDataAsChanged()
	{
		MarkCacheDataAsChanged();
	}

	AGraphicResourceView::ResourceTypeShared AGraphicResourceView::CreateDefaultInternalResource() const
	{
		Asserts::CheckBool(false, "AGraphicResourceView::CreateDefaultInternalResource is not implemented");
		return {};
	}

	void AGraphicResourceView::RefreshInternalResourceFromSelf(ResourceTypeShared const&, CacheFlagType const dirtyFlags)
	{
		// Do nothing.
	}

	void AGraphicResourceView::InitializeInternalResource()
	{
		FlushCacheDataExecutors().connect(boost::bind(&ThisType::ExecuteFlushForInternalResource, this, boost::placeholders::_1));
	}

	void AGraphicResourceView::TerminateInternalResource()
	{
		FlushCacheDataExecutors().disconnect(boost::bind(&ThisType::ExecuteFlushForInternalResource, this, boost::placeholders::_1));
	}

	void AGraphicResourceView::ExecuteFlushForInternalResource(CacheFlagType const dirtyFlags)
	{
		RefreshInternalResourceFromSelf(GetOrAddInternalResource(), dirtyFlags);
	}
}
