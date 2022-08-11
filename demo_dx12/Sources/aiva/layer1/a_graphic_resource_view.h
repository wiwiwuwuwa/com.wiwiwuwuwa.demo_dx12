#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view_fwd.h>
#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/e_resource_view_type.h>
#include <aiva/layer1/i_object_engineable.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_cacheable.h>

namespace aiva::layer1
{
	struct AGraphicResource;
}

namespace aiva::layer1
{
	struct AGraphicResourceView : public aiva::utils::AObject, public aiva::utils::IObjectCacheable, public aiva::layer1::IObjectEngineable
	{
	// ----------------------------------------------------
	// Main

	protected:
		AGraphicResourceView(EngineType const& engine);

	public:
		~AGraphicResourceView() override;

	// ----------------------------------------------------
	// Internal Resource

	public:
		using ResourceType = AGraphicResource;

	public:
		std::shared_ptr<ResourceType> GetInternalResource();

		std::shared_ptr<ResourceType> GetOrAddInternalResource();

		AGraphicResourceView& SetInternalResource(std::shared_ptr<ResourceType> const resource);

	protected:
		virtual std::shared_ptr<ResourceType> CreateDefaultInternalResource() const;

		virtual void RefreshInternalResourceFromSelf(std::shared_ptr<ResourceType> const& resource);

	private:
		void InitializeInternalResource();

		void TerminateInternalResource();

	private:
		void ExecuteFlushForInternalResource();

		void ExecuteMarkAsChangedForSelf();

	private:
		std::shared_ptr<ResourceType> mInternalResource{};

	// ----------------------------------------------------
	// Metadata

	public:
		virtual EDescriptorHeapType HeapType() const = 0;

		virtual EResourceViewType ViewType() const = 0;

	// ----------------------------------------------------
	// DirectX

	public:
		virtual void CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) = 0;

		virtual std::vector<D3D12_RESOURCE_BARRIER> CreateDirectxBarriers(bool const active) = 0;
	};
}
