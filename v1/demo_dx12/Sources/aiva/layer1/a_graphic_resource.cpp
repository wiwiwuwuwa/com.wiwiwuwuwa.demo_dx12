#include <pch.h>
#include <aiva/layer1/a_graphic_resource.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/resource_barrier.h>
#include <aiva/utils/t_cache_updater.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	AGraphicResource::AGraphicResource(EngineType const& engine)
		: AObject{}, IObjectCacheable{ true }, IObjectEngineable{ engine }
	{
		InitializeInternalResource();
		InitializeResourceBarrier();
	}

	AGraphicResource::~AGraphicResource()
	{
		TerminateResourceBarrier();
		TerminateInternalResource();
	}

	AGraphicResource::ResourceType const& AGraphicResource::GetInternalResource()
	{
		FlushCacheDataChanges();
		return mInternalResource;
	}

	void AGraphicResource::SetInternalResource(ResourceType const& resource)
	{
		if (resource)
		{
			mInternalResource = resource;
			RefreshSelfFromInternalResource(mInternalResource);

			MarkCacheDataAsChanged();
			SkipCacheDataChanges();
		}
		else
		{
			mInternalResource = {};
			MarkCacheDataAsChanged();
		}
	}

	void AGraphicResource::InitializeInternalResource()
	{
		FlushCacheDataExecutors().connect(boost::bind(&AGraphicResource::FlushInternalResource, this));
	}

	void AGraphicResource::TerminateInternalResource()
	{
		FlushCacheDataExecutors().disconnect(boost::bind(&AGraphicResource::FlushInternalResource, this));
	}

	void AGraphicResource::FlushInternalResource()
	{
		mInternalResource = {};
		Asserts::CheckBool(mResourceBarrier, "Resource barrier is not valid");

		RefreshInternalResourceFromSelf(mInternalResource, *mResourceBarrier);
		Asserts::CheckBool(mInternalResource, "Internal resource is not valid");
	}

	std::vector<D3D12_RESOURCE_BARRIER> AGraphicResource::CreateDirectxBarriers(D3D12_RESOURCE_STATES const desiredState, std::optional<std::size_t> const subresource /*= {}*/)
	{
		auto const& internalResource = GetInternalResource();
		Asserts::CheckBool(internalResource, "Internal resource is not valid");

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

	void AGraphicResource::InitializeResourceBarrier()
	{
		mResourceBarrier = std::make_unique<ResourceBarrier>();
		Asserts::CheckBool(mResourceBarrier, "Resource barrier is not valid");
	}

	void AGraphicResource::TerminateResourceBarrier()
	{
		Asserts::CheckBool(mResourceBarrier, "Resource barrier is not valid");
		mResourceBarrier = {};
	}

}
