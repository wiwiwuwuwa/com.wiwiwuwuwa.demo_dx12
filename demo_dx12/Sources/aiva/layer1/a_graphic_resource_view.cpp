#include <pch.h>
#include <aiva/layer1/a_graphic_resource_view.h>

#include <aiva/layer1/a_graphic_resource.h>
#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>

aiva::layer1::AGraphicResourceView::AGraphicResourceView(EngineType const& engine)
	: aiva::utils::AObject{}, aiva::utils::IObjectCacheable{ true }, aiva::layer1::IObjectEngineable{ engine }
{
	InitializeInternalResource();
}

aiva::layer1::AGraphicResourceView::~AGraphicResourceView()
{
	TerminateInternalResource();
}

std::shared_ptr<aiva::layer1::AGraphicResourceView::ResourceType> const& aiva::layer1::AGraphicResourceView::InternalResource()
{
	FlushChanges();
	return mInternalResource;
}

void aiva::layer1::AGraphicResourceView::InternalResource(std::shared_ptr<ResourceType> const& resource)
{
	if (mInternalResource)
	{
		mInternalResource->OnMarkAsChanged().disconnect(boost::bind(&AGraphicResourceView::ExecuteMarkAsChangedForSelf, this));
	}

	mInternalResource = resource;

	if (mInternalResource)
	{
		mInternalResource->OnMarkAsChanged().connect(boost::bind(&AGraphicResourceView::ExecuteMarkAsChangedForSelf, this));
	}

	MarkAsChanged();
}

void aiva::layer1::AGraphicResourceView::RefreshInternalResourceFromSelf(std::shared_ptr<ResourceType> const& resource)
{

}

void aiva::layer1::AGraphicResourceView::InitializeInternalResource()
{
	FlushExecutors().connect(boost::bind(&AGraphicResourceView::ExecuteFlushForInternalResource, this));
}

void aiva::layer1::AGraphicResourceView::TerminateInternalResource()
{
	FlushExecutors().disconnect(boost::bind(&AGraphicResourceView::ExecuteFlushForInternalResource, this));
}

void aiva::layer1::AGraphicResourceView::ExecuteFlushForInternalResource()
{
	if (mInternalResource)
	{
		RefreshInternalResourceFromSelf(mInternalResource);
	}
}

void aiva::layer1::AGraphicResourceView::ExecuteMarkAsChangedForSelf()
{
	MarkAsChanged();
}
