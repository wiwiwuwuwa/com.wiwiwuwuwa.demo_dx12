#include <pch.h>
#include <aiva/layer1/a_graphic_resource.h>

#include <aiva/layer1/engine.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/resource_barrier.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::AGraphicResource::AGraphicResource(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
	InitializeInternalResource();
	InitializeResourceBarrier();
}

aiva::layer1::AGraphicResource::~AGraphicResource()
{
	TerminateResourceBarrier();
	TerminateInternalResource();
	TerminateCacheUpdater();
}

aiva::layer1::Engine const& aiva::layer1::AGraphicResource::Engine() const
{
	return mEngine;
}

aiva::layer1::AGraphicResource::CacheUpdaterType::ActionType& aiva::layer1::AGraphicResource::OnMarkAsChanged()
{
	return mCacheUpdater->OnMarkAsChanged();
}

void aiva::layer1::AGraphicResource::MarkAsChanged()
{
	mCacheUpdater->MarkAsChanged();
}

void aiva::layer1::AGraphicResource::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
}

void aiva::layer1::AGraphicResource::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater, "Cache updater is not valid");
	mCacheUpdater = {};
}

winrt::com_ptr<ID3D12Resource> const& aiva::layer1::AGraphicResource::InternalResource()
{
	mCacheUpdater->FlushChanges();
	return mInternalResource;
}

void aiva::layer1::AGraphicResource::InitializeInternalResource()
{
	mCacheUpdater->FlushExecutors().connect(boost::bind(&AGraphicResource::ExecuteInternalResourceFlush, this));
}

void aiva::layer1::AGraphicResource::TerminateInternalResource()
{
	mCacheUpdater->FlushExecutors().disconnect(boost::bind(&AGraphicResource::ExecuteInternalResourceFlush, this));
}

void aiva::layer1::AGraphicResource::ExecuteInternalResourceFlush()
{
	mInternalResource = {};

	RefreshInternalResource(mInternalResource, *mResourceBarrier);
	aiva::utils::Asserts::CheckBool(mInternalResource, "Internal resource is not valid");
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::AGraphicResource::PrepareBarriers(D3D12_RESOURCE_STATES const desiredState, std::optional<std::size_t> const subresource /*= {}*/)
{
	auto const internalResource = InternalResource();
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
