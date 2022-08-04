#include <pch.h>
#include <aiva/layer1/resource_view_table.h>

#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::ResourceViewTable::ResourceViewTable(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
}

aiva::layer1::ResourceViewTable::~ResourceViewTable()
{
	TerminateCacheUpdater();
}

aiva::layer1::ResourceViewTable::CacheUpdaterType& aiva::layer1::ResourceViewTable::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::ResourceViewTable::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::ResourceViewTable::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

std::shared_ptr<aiva::layer1::ResourceViewHeap> aiva::layer1::ResourceViewTable::GetResourceHeap(EDescriptorHeapType const key) const
{
	auto const& resourceHeapIter = mResourceHeaps.find(key);
	if (resourceHeapIter == mResourceHeaps.end())
	{
		return {};
	}

	return resourceHeapIter->second;
}

std::shared_ptr<aiva::layer1::ResourceViewHeap> aiva::layer1::ResourceViewTable::GetOrAddResourceHeap(EDescriptorHeapType const key)
{
	auto const& heapIter = mResourceHeaps.find(key);
	if (heapIter != mResourceHeaps.end())
	{
		return heapIter->second;
	}

	auto const& heapResource = ResourceViewHeap::Create(mEngine, key);
	aiva::utils::Asserts::CheckBool(heapResource);

	SetResourceHeap(key, heapResource);
	return heapResource;
}

aiva::layer1::ResourceViewTable& aiva::layer1::ResourceViewTable::SetResourceHeap(EDescriptorHeapType const key, std::shared_ptr<ResourceViewHeap> const& value)
{
	auto const& previousHeapIter = mResourceHeaps.find(key);
	if (previousHeapIter != mResourceHeaps.end())
	{
		auto const& previousHeapObject = previousHeapIter->second;
		aiva::utils::Asserts::CheckBool(previousHeapObject);

		previousHeapObject->CacheUpdater().OnMarkAsChanged().disconnect(boost::bind(&ResourceViewTable::OnResourceHeapMarkedAsChanged, this));
		mResourceHeaps.erase(previousHeapIter);
	}

	if (!value)
	{
		CacheUpdater().MarkAsChanged();
		return *this;
	}

	auto const& currentHeapIter = mResourceHeaps.insert_or_assign(key, value).first;
	if (currentHeapIter != mResourceHeaps.end())
	{
		auto const& currentHeapObject = currentHeapIter->second;
		aiva::utils::Asserts::CheckBool(currentHeapObject);

		currentHeapObject->CacheUpdater().OnMarkAsChanged().connect(boost::bind(&ResourceViewTable::OnResourceHeapMarkedAsChanged, this));
	}

	CacheUpdater().MarkAsChanged();
	return *this;
}

aiva::layer1::ResourceViewTable::ResourceHeapMap const& aiva::layer1::ResourceViewTable::ResourceHeaps() const
{
	return mResourceHeaps;
}

void aiva::layer1::ResourceViewTable::OnResourceHeapMarkedAsChanged()
{
	CacheUpdater().MarkAsChanged();
}

std::vector<winrt::com_ptr<ID3D12DescriptorHeap>> aiva::layer1::ResourceViewTable::InternalResource() const
{
	CacheUpdater().FlushChanges();

	auto heapsPerTypes = std::vector<ResourceHeapMap::const_iterator>();
	for (auto iter = mResourceHeaps.cbegin(); iter != mResourceHeaps.cend(); iter++) heapsPerTypes.emplace_back(iter);
	std::sort(heapsPerTypes.begin(), heapsPerTypes.end(), [](auto const& a, auto const& b) { return a->first < b->first; });

	std::for_each(heapsPerTypes.cbegin(), heapsPerTypes.cend(), [](auto const& iter) { aiva::utils::Asserts::CheckBool(iter->second); });

	auto tableResource = std::vector<winrt::com_ptr<ID3D12DescriptorHeap>>{};
	std::transform(heapsPerTypes.cbegin(), heapsPerTypes.cend(), std::back_inserter(tableResource), [](auto const& iter) { return iter->second->InternalDescriptorHeap(); });

	std::for_each(tableResource.cbegin(), tableResource.cend(), [](auto const& res) { winrt::check_bool(res); });
	return tableResource;
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::ResourceViewTable::PrepareBarriers(bool const active) const
{
	CacheUpdater().FlushChanges();

	auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

	auto const& resourceHeaps = ResourceHeaps();
	std::for_each(std::cbegin(resourceHeaps), std::cend(resourceHeaps), [&barriers, active](auto const resourceHeap)
	{
			aiva::utils::Asserts::CheckBool(resourceHeap.second);

			auto const& resourceBarriers = resourceHeap.second->PrepareBarriers(active);
			std::copy(std::cbegin(resourceBarriers), std::cend(resourceBarriers), std::back_inserter(barriers));
	});

	return barriers;
}

void aiva::layer1::ResourceViewTable::CopyPropertiesFrom(ResourceViewTable const& source)
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
		auto const copiedResourceHeap = ResourceViewHeap::Create(mEngine);
		copiedResourceHeap->CopyPropertiesFrom(*sourceResourceHeap.second);

		SetResourceHeap(sourceResourceHeap.first, copiedResourceHeap);
	}

	CacheUpdater().MarkAsChanged();
}
