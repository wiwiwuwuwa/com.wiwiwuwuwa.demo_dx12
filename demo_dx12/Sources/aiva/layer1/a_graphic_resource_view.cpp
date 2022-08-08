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

std::shared_ptr<aiva::layer1::AGraphicResourceView::ResourceType> aiva::layer1::AGraphicResourceView::GetInternalResource()
{
	FlushChanges();
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
		mInternalResource->OnMarkAsChanged().disconnect(boost::bind(&AGraphicResourceView::ExecuteMarkAsChangedForSelf, this));
	}

	mInternalResource = resource;

	if (mInternalResource)
	{
		mInternalResource->OnMarkAsChanged().connect(boost::bind(&AGraphicResourceView::ExecuteMarkAsChangedForSelf, this));
	}

	MarkAsChanged();
	return *this;
}

std::shared_ptr<aiva::layer1::AGraphicResourceView::ResourceType> aiva::layer1::AGraphicResourceView::CreateDefaultInternalResource() const
{
	return {};
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
	RefreshInternalResourceFromSelf(GetOrAddInternalResource());
}

void aiva::layer1::AGraphicResourceView::ExecuteMarkAsChangedForSelf()
{
	MarkAsChanged();
}
