#include <pch.h>
#include <aiva/layer1_ext/rt_table_main.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/rt_resource_heap_container.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	RtTableMain::RtTableMain(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		InitGenericFields();
		InitNativeResourceFields();
		InitResourceHeapsFields();
	}

	RtTableMain::~RtTableMain()
	{
		ShutResourceHeapsFields();
		ShutNativeResourceFields();
		ShutGenericFields();
	}

	void RtTableMain::InitGenericFields()
	{
		mDirtyFlagsForNativeResource.OnAfterDirtied().AttachListener(&ThisType::GenericFields_When_NativeResourceFields_OnDirtied, this);
	}

	void RtTableMain::ShutGenericFields()
	{
		mDirtyFlagsForNativeResource.OnAfterDirtied().RemoveListener(&ThisType::GenericFields_When_NativeResourceFields_OnDirtied, this);
	}

	void RtTableMain::GenericFields_When_NativeResourceFields_OnDirtied()
	{
		mDirtyFlags.Dirty();
	}

	std::vector<winrt::com_ptr<ID3D12DescriptorHeap>> RtTableMain::NativeResource()
	{
		auto resources = std::vector<winrt::com_ptr<ID3D12DescriptorHeap>>{};

		for (auto const& pair : mResourceHeaps)
		{
			auto const& heap = pair.second;
			Asserts::CheckBool(heap, "Heap is not valid");

			auto const& data = heap->GetResourceHeapData();
			Asserts::CheckBool(data, "data is not valid");

			resources.emplace_back(data);
		}

		return resources;
	}

	void RtTableMain::InitNativeResourceFields()
	{
		mDirtyFlagsForResourceHeaps.OnAfterDirtied().AttachListener(&ThisType::NativeResourceFields_When_ResourceHeapsFields_OnDirtied, this);
	}

	void RtTableMain::ShutNativeResourceFields()
	{
		mDirtyFlagsForResourceHeaps.OnAfterDirtied().RemoveListener(&ThisType::NativeResourceFields_When_ResourceHeapsFields_OnDirtied, this);
	}

	void RtTableMain::NativeResourceFields_When_ResourceHeapsFields_OnDirtied()
	{
		mDirtyFlagsForNativeResource.Dirty();
	}

	void RtTableMain::InitResourceHeapsFields()
	{
		ClearResourceHeaps();
	}

	void RtTableMain::ShutResourceHeapsFields()
	{
		ClearResourceHeaps();
	}

	bool RtTableMain::ContainResourceHeap(RtResourceHeapType const heapType) const
	{
		auto const iter = mResourceHeaps.find(heapType);
		return iter != std::cend(mResourceHeaps);
	}

	std::shared_ptr<RtResourceHeapContainer> const& RtTableMain::AccessResourceHeap(RtResourceHeapType const heapType)
	{
		auto const iter = mResourceHeaps.find(heapType);
		Asserts::CheckBool(iter != std::cend(mResourceHeaps), "Iter is not valid");

		auto const& heap = iter->second;
		Asserts::CheckBool(heap, "Heap is not valid");

		return heap;
	}

	void RtTableMain::InsertResourceHeap(RtResourceHeapType const heapType, std::shared_ptr<RtResourceHeapContainer> const& resourceHeap)
	{
		Asserts::CheckBool(resourceHeap, "Resource heap is not valid");
		Asserts::CheckBool(!ContainResourceHeap(heapType), "Resource heap is already inserted");

		mResourceHeaps.insert_or_assign(heapType, resourceHeap);
		resourceHeap->GetResourceHeapDirtyFlags().OnAfterDirtied().AttachListener(&ThisType::ResourceHeapsFields_When_ResourceHeap_OnDirtied, this);
	}

	void RtTableMain::RemoveResourceHeap(RtResourceHeapType const heapType)
	{
		auto const iter = mResourceHeaps.find(heapType);
		Asserts::CheckBool(iter != std::cend(mResourceHeaps), "Iter is not valid");

		auto const& heap = iter->second;
		Asserts::CheckBool(heap, "Heap is not valid");

		heap->GetResourceHeapDirtyFlags().OnAfterDirtied().RemoveListener(&ThisType::ResourceHeapsFields_When_ResourceHeap_OnDirtied, this);
		mResourceHeaps.erase(iter);
	}

	void RtTableMain::ClearResourceHeaps()
	{
		while (!std::empty(mResourceHeaps))
		{
			auto const iter = std::crbegin(mResourceHeaps);
			Asserts::CheckBool(iter != std::crend(mResourceHeaps), "Iter is not valid");

			RemoveResourceHeap(iter->first);
		}

		mDirtyFlagsForResourceHeaps.Dirty();
	}

	void RtTableMain::ResourceHeapsFields_When_ResourceHeap_OnDirtied()
	{
		mDirtyFlagsForResourceHeaps.Dirty();
	}

	std::vector<D3D12_RESOURCE_BARRIER> RtTableMain::TransiteResourceBarriers(UtBarriersFlags const flags)
	{
		auto resourcesBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		for (auto const& pair : mResourceHeaps)
		{
			auto const& heap = pair.second;
			Asserts::CheckBool(heap, "Heap is not valid");

			auto const barriers = heap->TransiteResourceBarriers(flags);
			std::copy(std::cbegin(barriers), std::cend(barriers), std::back_inserter(resourcesBarriers));
		}

		return resourcesBarriers;
	}
}
