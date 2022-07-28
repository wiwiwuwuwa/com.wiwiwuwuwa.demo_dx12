#include <pch.h>
#include <aiva/layer1/resource_view_heap.h>

#include <aiva/layer1/e_gpu_descriptor_heap_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::ResourceViewHeap::ResourceViewHeap(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
	InitializeInternalResources();
}

aiva::layer1::ResourceViewHeap::~ResourceViewHeap()
{
	TerminateInternalResources();
	TerminateCacheUpdater();
}

aiva::layer1::ResourceViewHeap::CacheUpdaterType& aiva::layer1::ResourceViewHeap::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::ResourceViewHeap::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::ResourceViewHeap::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

aiva::layer1::EGpuDescriptorHeapType aiva::layer1::ResourceViewHeap::ResourceType() const
{
	return mResourceType;
}

aiva::layer1::ResourceViewHeap& aiva::layer1::ResourceViewHeap::ResourceType(EGpuDescriptorHeapType const resourceType)
{
	mResourceType = resourceType;
	mResourceViews = {};

	CacheUpdater().MarkAsChanged();
	return *this;
}

std::shared_ptr<aiva::layer1::IGpuResourceView> aiva::layer1::ResourceViewHeap::ResourceView(std::string const& key) const
{
	auto const& resourceViewIter = mResourceViews.find(key);
	if (resourceViewIter == mResourceViews.end())
	{
		return {};
	}

	return resourceViewIter->second;
}

aiva::layer1::ResourceViewHeap& aiva::layer1::ResourceViewHeap::ResourceView(std::string const& key, std::shared_ptr<IGpuResourceView> const& value)
{
	aiva::utils::Asserts::CheckBool(!value || value->HeapType() == ResourceType());

	auto const& previousResourceIter = mResourceViews.find(key);
	if (previousResourceIter != mResourceViews.end())
	{
		auto const& previousResourceView = previousResourceIter->second;
		aiva::utils::Asserts::CheckBool(previousResourceView);

		previousResourceView->OnMarkAsChanged().disconnect(boost::bind(&ResourceViewHeap::OnResourceViewMarkedAsChanged, this));
		mResourceViews.erase(previousResourceIter);
	}

	if (!value)
	{
		CacheUpdater().MarkAsChanged();
		return *this;
	}

	auto const& currentResourceIter = mResourceViews.insert_or_assign(key, value).first;
	if (currentResourceIter != mResourceViews.end())
	{
		auto const& currentResourceView = currentResourceIter->second;
		aiva::utils::Asserts::CheckBool(currentResourceView);

		currentResourceView->OnMarkAsChanged().connect(boost::bind(&ResourceViewHeap::OnResourceViewMarkedAsChanged, this));
	}

	CacheUpdater().MarkAsChanged();
	return *this;
}

std::vector<std::shared_ptr<aiva::layer1::IGpuResourceView>> aiva::layer1::ResourceViewHeap::SortedResourceViews() const
{
	auto viewsPerNames = std::vector<decltype(mResourceViews)::const_iterator>();
	for (auto iter = mResourceViews.cbegin(); iter != mResourceViews.cend(); iter++) viewsPerNames.emplace_back(iter);
	std::sort(viewsPerNames.begin(), viewsPerNames.end(), [](auto const& a, auto const& b) { return a->first < b->first; });

	auto sortedViews = std::vector<decltype(viewsPerNames)::value_type::value_type::second_type>{};
	std::transform(viewsPerNames.cbegin(), viewsPerNames.cend(), std::back_inserter(sortedViews), [](auto const& iter) { return iter->second; });

	return sortedViews;
}

void aiva::layer1::ResourceViewHeap::OnResourceViewMarkedAsChanged()
{
	CacheUpdater().MarkAsChanged();
}

winrt::com_ptr<ID3D12DescriptorHeap> const& aiva::layer1::ResourceViewHeap::InternalResource() const
{
	CacheUpdater().FlushChanges();
	return mInternalResource;
}

void aiva::layer1::ResourceViewHeap::InitializeInternalResources()
{
	CacheUpdater().FlushExecutors().connect(boost::bind(&ResourceViewHeap::RefreshInternalResources, this));
}

void aiva::layer1::ResourceViewHeap::TerminateInternalResources()
{
	CacheUpdater().FlushExecutors().disconnect(boost::bind(&ResourceViewHeap::RefreshInternalResources, this));
}

void aiva::layer1::ResourceViewHeap::RefreshInternalResources()
{
	mInternalResource = !std::empty(mResourceViews) ? CreateInternalResource(mEngine, SortedResourceViews()) : decltype(mInternalResource){};
}

winrt::com_ptr<ID3D12DescriptorHeap> aiva::layer1::ResourceViewHeap::CreateInternalResource(Engine const& engine, std::vector<std::shared_ptr<IGpuResourceView>> const& resourceViews)
{
	auto const& device = engine.GraphicHardware().Device();
	winrt::check_bool(device);

	aiva::utils::Asserts::CheckBool(!std::empty(resourceViews));
	std::for_each(resourceViews.cbegin(), resourceViews.cend(), [](auto const& resourceView) { aiva::utils::Asserts::CheckBool(resourceView); });

	auto const heapType = resourceViews.front()->HeapType();
	std::for_each(resourceViews.cbegin(), resourceViews.cend(), [heapType](auto const& resourceView) { aiva::utils::Asserts::CheckBool(resourceView->HeapType() == heapType); });

	auto heapDesc = D3D12_DESCRIPTOR_HEAP_DESC{};
	heapDesc.Type = ToInternalEnum(heapType);
	heapDesc.NumDescriptors = std::size(resourceViews);
	heapDesc.Flags = (heapType == EGpuDescriptorHeapType::CbvSrvUav ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	heapDesc.NodeMask = 0;

	auto heapResource = winrt::com_ptr<ID3D12DescriptorHeap>();
	winrt::check_hresult(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heapResource)));

	auto const incrementSize = std::size_t{ device->GetDescriptorHandleIncrementSize(ToInternalEnum(heapType)) };
	for (std::size_t i = {}; i < resourceViews.size(); i++)
	{
		auto resourceHandle = heapResource->GetCPUDescriptorHandleForHeapStart();
		resourceHandle.ptr += i * incrementSize;

		resourceViews.at(i)->CreateView(resourceHandle);
	}

	return heapResource;
}
