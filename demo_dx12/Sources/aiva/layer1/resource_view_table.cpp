#include <pch.h>
#include <aiva/layer1/resource_view_table.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/object_utils.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	ResourceViewTable::ResourceViewTable(EngineType const& engine) : AObject{}, IObjectChangeable{}, IObjectEngineable{ engine }
	{

	}

	ResourceViewTable::~ResourceViewTable()
	{

	}

	ResourceViewHeapTypeShared ResourceViewTable::GetResourceHeap(EDescriptorHeapType const key) const
	{
		auto const& heapIter = mResourceHeaps.find(key);
		if (heapIter == mResourceHeaps.end())
		{
			return {};
		}

		auto const& heapValue = heapIter->second;
		Asserts::CheckBool(heapValue, "Heap value is not valid");

		return heapValue;
	}

	ResourceViewHeapTypeShared ResourceViewTable::GetOrAddResourceHeap(EDescriptorHeapType const key)
	{
		auto const& heapIter = mResourceHeaps.find(key);
		if (heapIter != mResourceHeaps.end())
		{
			auto const& existingHeap = heapIter->second;
			Asserts::CheckBool(existingHeap, "Existing heap is not valid");

			return existingHeap;
		}
		else
		{
			auto const createdHeap = NewObject<ResourceViewHeapType>(Engine(), key);
			Asserts::CheckBool(createdHeap, "Created heap is not valid");

			SetResourceHeap(key, createdHeap);
			return createdHeap;
		}
	}

	ResourceViewTable& ResourceViewTable::SetResourceHeap(EDescriptorHeapType const key, ResourceViewHeapTypeShared const& value)
	{
		auto const& previousHeapIter = mResourceHeaps.find(key);
		if (previousHeapIter != mResourceHeaps.end())
		{
			auto const& previousHeapValue = previousHeapIter->second;
			Asserts::CheckBool(previousHeapValue, "Previous heap value is not valid");

			previousHeapValue->OnMarkCacheDataAsChanged().disconnect(boost::bind(&ResourceViewTable::ResourceHeap_OnMarkedAsChanged, this));
			mResourceHeaps.erase(previousHeapIter);
			BroadcastCacheDataChanged();
		}

		if (!value)
		{
			return *this;
		}

		auto const& currentHeapIter = mResourceHeaps.insert_or_assign(key, value).first;
		if (currentHeapIter != mResourceHeaps.end())
		{
			auto const& currentHeapValue = currentHeapIter->second;
			Asserts::CheckBool(currentHeapValue, "Current heap value is not valid");

			currentHeapValue->OnMarkCacheDataAsChanged().connect(boost::bind(&ResourceViewTable::ResourceHeap_OnMarkedAsChanged, this));
			BroadcastCacheDataChanged();
		}

		return *this;
	}

	ResourceViewTable::HeapDict const& ResourceViewTable::ResourceHeaps() const
	{
		return mResourceHeaps;
	}

	void ResourceViewTable::ResourceHeap_OnMarkedAsChanged()
	{
		BroadcastCacheDataChanged();
	}

	std::vector<winrt::com_ptr<ID3D12DescriptorHeap>> ResourceViewTable::InternalResource() const
	{
		auto heapsPerTypes = std::vector<HeapDict::const_iterator>();
		for (auto iter = mResourceHeaps.cbegin(); iter != mResourceHeaps.cend(); iter++) heapsPerTypes.emplace_back(iter);
		std::sort(heapsPerTypes.begin(), heapsPerTypes.end(), [](auto const& a, auto const& b) { return a->first < b->first; });

		std::for_each(heapsPerTypes.cbegin(), heapsPerTypes.cend(), [](auto const& iter) { Asserts::CheckBool(iter->second, "Heap value is not valid"); });

		auto tableResource = std::vector<winrt::com_ptr<ID3D12DescriptorHeap>>{};
		std::transform(heapsPerTypes.cbegin(), heapsPerTypes.cend(), std::back_inserter(tableResource), [](auto const& iter) { return iter->second->InternalDescriptorHeap(); });

		std::for_each(tableResource.cbegin(), tableResource.cend(), [](auto const& res) { winrt::check_bool(res); });
		return tableResource;
	}

	std::vector<D3D12_RESOURCE_BARRIER> ResourceViewTable::PrepareBarriers(bool const active) const
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		for (auto const& pair : ResourceHeaps())
		{
			auto const& heapValue = pair.second;
			Asserts::CheckBool(heapValue, "Heap value is not valid");

			auto const heapBarriers = heapValue->PrepareBarriers(active);
			std::copy(std::cbegin(heapBarriers), std::cend(heapBarriers), std::back_inserter(barriers));
		}

		return barriers;
	}

	void ResourceViewTable::CopyPropertiesFrom(ResourceViewTable const& source)
	{
		auto keysToRemove = std::vector<EDescriptorHeapType>{};
		for (auto const& resourceHeap : ResourceHeaps())
		{
			keysToRemove.emplace_back(resourceHeap.first);
		}
		for (auto const& keyToRemove : keysToRemove)
		{
			SetResourceHeap(keyToRemove, {});
		}

		for (auto const& sourceResourceHeap : source.ResourceHeaps())
		{
			auto const copiedResourceHeap = NewObject<ResourceViewHeapType>(Engine());
			copiedResourceHeap->CopyPropertiesFrom(*sourceResourceHeap.second);

			SetResourceHeap(sourceResourceHeap.first, copiedResourceHeap);
		}

		BroadcastCacheDataChanged();
	}
}