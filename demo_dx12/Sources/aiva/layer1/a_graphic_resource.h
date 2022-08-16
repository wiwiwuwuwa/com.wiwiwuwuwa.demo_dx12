#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_fwd.h>
#include <aiva/layer1/i_object_engineable.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_cacheable.h>

namespace aiva::utils
{
	struct ResourceBarrier;
}

namespace aiva::layer1
{
	struct AGraphicResource : public aiva::utils::AObject, public aiva::utils::IObjectCacheable, public aiva::layer1::IObjectEngineable
	{
	// ----------------------------------------------------
	// Main

	protected:
		AGraphicResource(EngineType const& engine);

	public:
		~AGraphicResource() override;

	// ----------------------------------------------------
	// Aliases

	public:
		using ResourceType = winrt::com_ptr<ID3D12Resource>;

		using BarrierType = aiva::utils::ResourceBarrier;

	// ----------------------------------------------------
	// Internal Resource

	public:
		ResourceType const& GetInternalResource();

		void SetInternalResource(ResourceType const& resource);

	protected:
		virtual void RefreshInternalResourceFromSelf(ResourceType& resource, BarrierType& barrier) = 0;

		virtual void RefreshSelfFromInternalResource(ResourceType const& resource) = 0;

	private:
		void InitializeInternalResource();

		void TerminateInternalResource();

	private:
		void FlushInternalResource();

	private:
		ResourceType mInternalResource{};

	// ----------------------------------------------------
	// Resource Barriers

	public:
		std::vector<D3D12_RESOURCE_BARRIER> CreateDirectxBarriers(D3D12_RESOURCE_STATES const desiredState, std::optional<std::size_t> const subresource = {});

	private:
		void InitializeResourceBarrier();

		void TerminateResourceBarrier();

	private:
		std::unique_ptr<aiva::utils::ResourceBarrier> mResourceBarrier{};
	};
}
