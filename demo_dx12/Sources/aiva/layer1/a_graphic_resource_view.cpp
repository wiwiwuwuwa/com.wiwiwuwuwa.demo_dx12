#include <pch.h>
#include <aiva/layer1/a_graphic_resource_view.h>

#include <aiva/layer1/a_graphic_resource.h>
#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>

aiva::layer1::AGraphicResourceView::AGraphicResourceView(aiva::layer1::Engine const& engine) : AGraphicObject{ engine }
{
	InitializeInternalResource();
}

aiva::layer1::AGraphicResourceView::~AGraphicResourceView()
{
	TerminateInternalResource();
}

std::shared_ptr<aiva::layer1::AGraphicResource> const& aiva::layer1::AGraphicResourceView::InternalResource()
{
	FlushChanges();
	return mInternalResource;
}

void aiva::layer1::AGraphicResourceView::InitializeInternalResource()
{
	FlushExecutors().connect(boost::bind(&AGraphicResourceView::ExecuteFlushForInternalResource, this));
}

void aiva::layer1::AGraphicResourceView::TerminateInternalResource()
{
	FlushExecutors().disconnect(boost::bind(&AGraphicResourceView::ExecuteFlushForInternalResource, this));
}

void aiva::layer1::AGraphicResourceView::InternalResource(std::shared_ptr<AGraphicResource> const& resource)
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

void aiva::layer1::AGraphicResourceView::ExecuteFlushForInternalResource()
{
	if (mInternalResource)
	{
		mInternalResource->FlushChanges();
	}
}

void aiva::layer1::AGraphicResourceView::ExecuteMarkAsChangedForSelf()
{
	MarkAsChanged();
}
