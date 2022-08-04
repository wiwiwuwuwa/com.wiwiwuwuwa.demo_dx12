#include <pch.h>
#include <aiva/layer1/resource_view_heap.h>

#include <aiva/layer1/e_descriptor_heap_type.h>
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

aiva::layer1::ResourceViewHeap::ResourceViewHeap(Engine const& engine, EDescriptorHeapType const resourceType) : ResourceViewHeap(engine)
{
	ResourceType(resourceType);
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

aiva::layer1::EDescriptorHeapType aiva::layer1::ResourceViewHeap::ResourceType() const
{
	return mResourceType;
}

aiva::layer1::ResourceViewHeap& aiva::layer1::ResourceViewHeap::ResourceType(EDescriptorHeapType const resourceType)
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

aiva::layer1::ResourceViewHeap::ResourceViewsMap const& aiva::layer1::ResourceViewHeap::ResourceViews() const
{
	return mResourceViews;
}

void aiva::layer1::ResourceViewHeap::OnResourceViewMarkedAsChanged()
{
	CacheUpdater().MarkAsChanged();
}

winrt::com_ptr<ID3D12DescriptorHeap> const& aiva::layer1::ResourceViewHeap::InternalDescriptorHeap() const
{
	CacheUpdater().FlushChanges();
	return mInternalDescriptorHeap;
}

std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> const& aiva::layer1::ResourceViewHeap::InternalDescriptorHandles() const
{
	CacheUpdater().FlushChanges();
	return mInternalDescriptorHandles;
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
	RefreshInternalDescriptorHeap();
	RefreshInternalDescriptorHandles();
}

void aiva::layer1::ResourceViewHeap::RefreshInternalDescriptorHeap()
{
	if (std::empty(mResourceViews))
	{
		mInternalDescriptorHeap = {};
		return;
	}

	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto heapDesc = D3D12_DESCRIPTOR_HEAP_DESC{};
	heapDesc.Type = ToInternalEnum(ResourceType());
	heapDesc.NumDescriptors = std::size(mResourceViews);
	heapDesc.Flags = (ResourceType() == EDescriptorHeapType::CbvSrvUav ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	heapDesc.NodeMask = 0;

	auto heapResource = winrt::com_ptr<ID3D12DescriptorHeap>();
	winrt::check_hresult(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heapResource)));

	winrt::check_bool(heapResource);
	mInternalDescriptorHeap = heapResource;
}

void aiva::layer1::ResourceViewHeap::RefreshInternalDescriptorHandles()
{
	mInternalDescriptorHandles = {};

	if (!mInternalDescriptorHeap)
	{
		return;
	}

	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto const& resourceViews = ResourceViews();
	aiva::utils::Asserts::CheckBool(!resourceViews.empty());

	auto const incrementSize = std::size_t{ device->GetDescriptorHandleIncrementSize(ToInternalEnum(ResourceType())) };
	for (std::size_t i = {}; i < resourceViews.size(); i++)
	{
		auto const& resourceIter = std::next(resourceViews.cbegin(), i);
		aiva::utils::Asserts::CheckBool(resourceIter != resourceViews.end());

		auto const& resourceView = resourceIter->second;
		aiva::utils::Asserts::CheckBool(resourceView);

		auto resourceHandle = mInternalDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		resourceHandle.ptr += i * incrementSize;

		resourceView->CreateView(resourceHandle);
		mInternalDescriptorHandles.emplace_back(resourceHandle);
	}
}

std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> aiva::layer1::ResourceViewHeap::InternalDescriptorHandle(std::string const& viewKey) const
{
	CacheUpdater().FlushChanges();

	auto const& viewIter = ResourceViews().find(viewKey);
	if (viewIter == ResourceViews().end())
	{
		return {};
	}

	auto const& viewIndex = std::distance(ResourceViews().cbegin(), viewIter);
	aiva::utils::Asserts::CheckBool(viewIndex >= 0 & viewIndex < std::size(InternalDescriptorHandles()));

	return InternalDescriptorHandles().at(viewIndex);
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::ResourceViewHeap::PrepareBarriers(bool const active) const
{
	CacheUpdater().FlushChanges();

	auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

	auto const& resourceViews = ResourceViews();
	std::for_each(std::cbegin(resourceViews), std::cend(resourceViews), [&barriers, active](auto const& resourceView)
	{
		aiva::utils::Asserts::CheckBool(resourceView.second);

		auto const& resourceBarriers = resourceView.second->PrepareBarriers(active);
		std::copy(std::cbegin(resourceBarriers), std::cend(resourceBarriers), std::back_inserter(barriers));
	});

	return barriers;
}

void aiva::layer1::ResourceViewHeap::CopyPropertiesFrom(ResourceViewHeap const& source)
{
	ResourceType(source.ResourceType());

	auto keysToRemove = std::vector<std::string>{};
	for (auto const& resourceView : ResourceViews())
	{
		keysToRemove.emplace_back(resourceView.first);
	}
	for (auto const& keyToRemove : keysToRemove)
	{
		ResourceView(keyToRemove, {});
	}

	for (auto const& sourceResourceView : source.ResourceViews())
	{
		ResourceView(sourceResourceView.first, sourceResourceView.second);
	}

	CacheUpdater().MarkAsChanged();
}
