#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/rt_find_view_layout_a.h>
#include <aiva/layer1_ext/rt_find_view_layout_b.h>
#include <aiva/layer1_ext/rt_find_view_params_b.h>
#include <aiva/layer1_ext/rt_resource_heap_container_fwd.h>
#include <aiva/layer1_ext/rt_resource_heap_type.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>
#include <aiva/layer1_ext/rt_table_heap_fwd.h>
#include <aiva/layer1_ext/ut_resources_barriers_emitter.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_eager.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct RtTableMain final : public aiva::utils_ext::TObject<RtTableMain>, public UtResourcesBarriersEmitter
	{
	// ----------------------------------------------------
	// Main

	public:
		template <RtResourceHeapType TResourceHeapType>
		using LayoutTypeA = RtFindViewLayoutAT<TResourceHeapType>;

		template <RtResourceViewType TResourceViewType>
		using LayoutTypeB = RtFindViewLayoutBT<TResourceViewType>;

		template <typename TResourceClass, RtResourceViewType TResourceViewType>
		using ParamsType = RtFindViewParamsBT<TResourceClass, TResourceViewType>;

	public:
		M_OBJECT_BODY(RtTableMain);

	protected:
		RtTableMain(aiva::layer1::Engine& engine);

	public:
		~RtTableMain() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER();

	private:
		void InitGenericFields();

		void ShutGenericFields();

	private:
		void GenericFields_When_NativeResourceFields_OnDirtied();

	// ----------------------------------------------------
	// Native Resource Fields

	public:
		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER_2(DirtyFlagsForNativeResource, private);

	public:
		std::vector<winrt::com_ptr<ID3D12DescriptorHeap>> NativeResource();

	private:
		void InitNativeResourceFields();

		void ShutNativeResourceFields();

	private:
		void NativeResourceFields_When_ResourceHeapsFields_OnDirtied();

	// ----------------------------------------------------
	// Resource Heaps Fields

	public:
		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER_2(DirtyFlagsForResourceHeaps, private);

		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::map<RtResourceHeapType, std::shared_ptr<RtResourceHeapContainer>>), ResourceHeaps);

	public:
		template <RtResourceViewType TResourceViewType, typename TResourceClass>
		void InsertResource(std::shared_ptr<TResourceClass> const& resource, LayoutTypeB<TResourceViewType> const& layout, ParamsType<TResourceClass, TResourceViewType> const& params);

		template <RtResourceHeapType TResourceHeapType>
		void RemoveResource(LayoutTypeA<TResourceHeapType> const& layout);

	private:
		void InitResourceHeapsFields();

		void ShutResourceHeapsFields();

	private:
		template <RtResourceHeapType TResourceHeapType>
		std::shared_ptr<RtTableHeap<TResourceHeapType>> AccessOrCreateResourceHeap();

		bool ContainResourceHeap(RtResourceHeapType const heapType) const;

		template <RtResourceHeapType TResourceHeapType>
		std::shared_ptr<RtTableHeap<TResourceHeapType>> AccessResourceHeap();

		std::shared_ptr<RtResourceHeapContainer> const& AccessResourceHeap(RtResourceHeapType const heapType);

		void InsertResourceHeap(RtResourceHeapType const heapType, std::shared_ptr<RtResourceHeapContainer> const& resourceHeap);

		void RemoveResourceHeap(RtResourceHeapType const heapType);

		void ClearResourceHeaps();

	private:
		void ResourceHeapsFields_When_ResourceHeap_OnDirtied();

	// ----------------------------------------------------
	// Resources Barriers Emitter

	public:
		std::vector<D3D12_RESOURCE_BARRIER> TransiteResourceBarriers(UtBarriersFlags const flags) override;
	};
}

// --------------------------------------------------------

#include <aiva/layer1_ext/rt_table_heap.h>
#include <aiva/layer1_ext/rt_find_resource_heap_type.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	template <RtResourceViewType TResourceViewType, typename TResourceClass>
	void RtTableMain::InsertResource(std::shared_ptr<TResourceClass> const& resource, LayoutTypeB<TResourceViewType> const& layout, ParamsType<TResourceClass, TResourceViewType> const& params)
	{
		auto const heap = AccessOrCreateResourceHeap<RtFindResourceHeapTypeV<TResourceViewType>>();
		aiva::utils::Asserts::CheckBool(heap, "Heap is not valid");

		heap->InsertResource<TResourceViewType>(resource, layout, params);
	}

	template <RtResourceHeapType TResourceHeapType>
	void RtTableMain::RemoveResource(LayoutTypeA<TResourceHeapType> const& layout)
	{
		auto const heap = AccessResourceHeap<TResourceHeapType>();
		aiva::utils::Asserts::CheckBool(heap, "Heap is not valid");

		heap->RemoveResource(layout);

		if (heap->HasResources())
		{
			return;
		}

		RemoveResourceHeap(TResourceHeapType);
	}

	template <RtResourceHeapType TResourceHeapType>
	std::shared_ptr<RtTableHeap<TResourceHeapType>> RtTableMain::AccessOrCreateResourceHeap()
	{
		if (ContainResourceHeap(TResourceHeapType))
		{
			auto const heap = AccessResourceHeap<TResourceHeapType>();
			aiva::utils::Asserts::CheckBool(heap, "Heap is not valid");

			return heap;
		}
		else
		{
			auto const heap = aiva::utils_ext::NewObject<RtTableHeap<TResourceHeapType>>(mEngine);
			aiva::utils::Asserts::CheckBool(heap, "Heap is not valid");

			InsertResourceHeap(TResourceHeapType, heap);
			return heap;
		}
	}

	template <RtResourceHeapType TResourceHeapType>
	std::shared_ptr<RtTableHeap<TResourceHeapType>> RtTableMain::AccessResourceHeap()
	{
		auto const& basicHeap = AccessResourceHeap(TResourceHeapType);
		aiva::utils::Asserts::CheckBool(basicHeap, "Basic heap is not valid");

		auto const specificHeap = std::dynamic_pointer_cast<RtTableHeap<TResourceHeapType>>(basicHeap);
		aiva::utils::Asserts::CheckBool(specificHeap, "Specific heap is not valid");

		return specificHeap;
	}
}
