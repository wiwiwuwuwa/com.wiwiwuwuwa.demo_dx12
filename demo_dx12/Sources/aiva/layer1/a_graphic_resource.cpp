#include <pch.h>
#include <aiva/layer1/a_graphic_resource.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/resource_barrier.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::AGraphicResource::AGraphicResource(EngineType const& engine)
	: aiva::utils::AObject{}, aiva::utils::IObjectCacheable{ true }, aiva::layer1::IObjectEngineable{ engine }
{
	InitializeInternalResource();
	InitializeResourceBarrier();
}

aiva::layer1::AGraphicResource::~AGraphicResource()
{
	TerminateResourceBarrier();
	TerminateInternalResource();
}

winrt::com_ptr<ID3D12Resource> const& aiva::layer1::AGraphicResource::InternalResource()
{
	FlushCacheDataChanges();
	return mInternalResource;
}

void aiva::layer1::AGraphicResource::InternalResource(winrt::com_ptr<ID3D12Resource> const& resource)
{
	winrt::check_bool(resource);

	if (mInternalResource == resource)
	{
		return;
	}

	mInternalResource = resource;
	RefreshSelfFromInternalResource(resource);

	MarkCacheDataAsChanged();
	SkipCacheDataChanges();
}

void aiva::layer1::AGraphicResource::InitializeInternalResource()
{
	FlushCacheDataExecutors().connect(boost::bind(&AGraphicResource::ExecuteFlushForInternalResource, this));
}

void aiva::layer1::AGraphicResource::TerminateInternalResource()
{
	FlushCacheDataExecutors().disconnect(boost::bind(&AGraphicResource::ExecuteFlushForInternalResource, this));
}

void aiva::layer1::AGraphicResource::ExecuteFlushForInternalResource()
{
	mInternalResource = {};

	RefreshInternalResourceFromSelf(mInternalResource, *mResourceBarrier);
	aiva::utils::Asserts::CheckBool(mInternalResource, "Internal resource is not valid");
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::AGraphicResource::PrepareBarriers(D3D12_RESOURCE_STATES const desiredState, std::optional<std::size_t> const subresource /*= {}*/)
{
	auto const& internalResource = InternalResource();
	aiva::utils::Asserts::CheckBool(internalResource, "Internal resource is not valid");

	auto resourceBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

	auto previousState = D3D12_RESOURCE_STATES{};
	if (mResourceBarrier->Transite(desiredState, subresource, previousState))
	{
		auto& resourceBarrier = resourceBarriers.emplace_back();
		resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		resourceBarrier.Transition.pResource = internalResource.get();
		resourceBarrier.Transition.Subresource = subresource ? *subresource : D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		resourceBarrier.Transition.StateBefore = previousState;
		resourceBarrier.Transition.StateAfter = desiredState;
	}

	return resourceBarriers;
}

void aiva::layer1::AGraphicResource::InitializeResourceBarrier()
{
	mResourceBarrier = std::make_unique<aiva::utils::ResourceBarrier>();
	aiva::utils::Asserts::CheckBool(mResourceBarrier, "Resource barrier is not valid");
}

void aiva::layer1::AGraphicResource::TerminateResourceBarrier()
{
	aiva::utils::Asserts::CheckBool(mResourceBarrier, "Resource barrier is not valid");
	mResourceBarrier = {};
}
