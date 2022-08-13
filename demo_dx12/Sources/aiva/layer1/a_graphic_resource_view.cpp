#include <pch.h>
#include <aiva/layer1/a_graphic_resource_view.h>

#include <aiva/layer1/a_graphic_resource.h>
#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>

aiva::layer1::AGraphicResourceView::AGraphicResourceView(EngineType const& engine)
	: aiva::utils::AObject{}, aiva::utils::TObjectCacheable<EGrvCacheFlags>{ true }, aiva::layer1::IObjectEngineable{ engine }
{
	InitializeInternalResource();
}

aiva::layer1::AGraphicResourceView::~AGraphicResourceView()
{
	TerminateInternalResource();
}

std::shared_ptr<aiva::layer1::AGraphicResourceView::ResourceType> aiva::layer1::AGraphicResourceView::GetInternalResource()
{
	FlushCacheDataChanges();
	return mInternalResource;
}

std::shared_ptr<aiva::layer1::AGraphicResourceView::ResourceType> aiva::layer1::AGraphicResourceView::GetOrAddInternalResource()
{
	auto const existingInternalResource = GetInternalResource();
	if (existingInternalResource)
	{
		return existingInternalResource;
	}

	auto const createdInternalResource = SetInternalResource(CreateDefaultInternalResource()).GetInternalResource();
	if (createdInternalResource)
	{
		return createdInternalResource;
	}

	aiva::utils::Asserts::CheckBool(false, "Failed to get or add internal resource");
	return {};
}

aiva::layer1::AGraphicResourceView& aiva::layer1::AGraphicResourceView::SetInternalResource(std::shared_ptr<ResourceType> const resource)
{
	if (mInternalResource)
	{
		mInternalResource->OnMarkCacheDataAsChanged().disconnect(boost::bind(&AGraphicResourceView::InternalResource_OnMarkCacheDataAsChanged, this));
	}

	mInternalResource = resource;

	if (mInternalResource)
	{
		mInternalResource->OnMarkCacheDataAsChanged().connect(boost::bind(&AGraphicResourceView::InternalResource_OnMarkCacheDataAsChanged, this));
	}

	MarkCacheDataAsChanged();
	return *this;
}

void aiva::layer1::AGraphicResourceView::InternalResource_OnMarkCacheDataAsChanged()
{
	MarkCacheDataAsChanged();
}

std::shared_ptr<aiva::layer1::AGraphicResourceView::ResourceType> aiva::layer1::AGraphicResourceView::CreateDefaultInternalResource() const
{
	aiva::utils::Asserts::CheckBool(false, "AGraphicResourceView::CreateDefaultInternalResource is not implemented");
	return {};
}

void aiva::layer1::AGraphicResourceView::RefreshInternalResourceFromSelf(std::shared_ptr<ResourceType> const&, EGrvCacheFlags const dirtyFlags)
{
	// Do nothing.
}

void aiva::layer1::AGraphicResourceView::InitializeInternalResource()
{
	FlushCacheDataExecutors().connect(boost::bind(&AGraphicResourceView::ExecuteFlushForInternalResource, this, boost::placeholders::_1));
}

void aiva::layer1::AGraphicResourceView::TerminateInternalResource()
{
	FlushCacheDataExecutors().disconnect(boost::bind(&AGraphicResourceView::ExecuteFlushForInternalResource, this, boost::placeholders::_1));
}

void aiva::layer1::AGraphicResourceView::ExecuteFlushForInternalResource(EGrvCacheFlags const dirtyFlags)
{
	RefreshInternalResourceFromSelf(GetOrAddInternalResource(), dirtyFlags);
}
