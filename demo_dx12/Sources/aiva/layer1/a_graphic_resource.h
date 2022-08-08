#pragma once
#include <pch.h>

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
	// Internal Resource

	public:
		winrt::com_ptr<ID3D12Resource> const& InternalResource();

		void InternalResource(winrt::com_ptr<ID3D12Resource> const& resource);

	protected:
		virtual void RefreshInternalResourceFromSelf(winrt::com_ptr<ID3D12Resource>& resource, aiva::utils::ResourceBarrier& barrier) = 0;

		virtual void RefreshSelfFromInternalResource(winrt::com_ptr<ID3D12Resource> const& resource) = 0;

	private:
		void InitializeInternalResource();

		void TerminateInternalResource();

	private:
		void ExecuteFlushForInternalResource();

	private:
		winrt::com_ptr<ID3D12Resource> mInternalResource{};

	// ----------------------------------------------------
	// Resource Barriers

	public:
		std::vector<D3D12_RESOURCE_BARRIER> PrepareBarriers(D3D12_RESOURCE_STATES const desiredState, std::optional<std::size_t> const subresource = {});

	private:
		void InitializeResourceBarrier();

		void TerminateResourceBarrier();

	private:
		std::unique_ptr<aiva::utils::ResourceBarrier> mResourceBarrier{};
	};
}
