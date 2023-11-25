#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_fwd.h>
#include <aiva/layer1/a_graphic_resource_view_fwd.h>
#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/e_grv_cache_flags.h>
#include <aiva/layer1/e_resource_view_type.h>
#include <aiva/layer1/i_object_engineable.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/t_object_cacheable.h>

namespace aiva::layer1
{
	struct AGraphicResourceView : public aiva::utils::AObject, public aiva::utils::TObjectCacheable<EGrvCacheFlags>, public aiva::layer1::IObjectEngineable
	{
	// ----------------------------------------------------
	// Main

	protected:
		AGraphicResourceView(EngineType const& engine);

	public:
		~AGraphicResourceView() override;

	// ----------------------------------------------------
	// Aliases

	public:
		using HeapTypeEnum = EDescriptorHeapType;

		using ResourceType = AGraphicResource;

		using ResourceTypeShared = std::shared_ptr<ResourceType>;

		using ResourceTypeWeak = std::weak_ptr<ResourceType>;

		using ThisType = AGraphicResourceView;

		using ViewTypeEnum = EResourceViewType;

	// ----------------------------------------------------
	// Internal Resource

	public:
		ResourceTypeShared const& GetInternalResource();

		ResourceTypeShared const& GetOrAddInternalResource();

		ThisType& SetInternalResource(ResourceTypeShared const& resource);

	private:
		void InternalResource_OnMarkCacheDataAsChanged();

	// --------------------------------

	protected:
		virtual ResourceTypeShared CreateDefaultInternalResource() const;

		virtual void RefreshInternalResourceFromSelf(ResourceTypeShared const& resource, CacheFlagType const dirtyFlags);

	private:
		void InitializeInternalResource();

		void TerminateInternalResource();

	private:
		void ExecuteFlushForInternalResource(CacheFlagType const dirtyFlags);

	private:
		ResourceTypeShared mInternalResource{};

	// ----------------------------------------------------
	// Metadata

	public:
		virtual HeapTypeEnum HeapType() const = 0;

		virtual ViewTypeEnum ViewType() const = 0;

	// ----------------------------------------------------
	// DirectX

	public:
		virtual void CreateDirectxView(D3D12_CPU_DESCRIPTOR_HANDLE const destination) = 0;

		virtual std::vector<D3D12_RESOURCE_BARRIER> CreateDirectxBarriers(bool const active) = 0;
	};
}
