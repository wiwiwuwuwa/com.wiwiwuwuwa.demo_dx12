#include <pch.h>
#include <aiva/layer1/resource_view_heap.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/enum_utils.h>

aiva::layer1::ResourceViewHeap::ResourceViewHeap(EngineType const& engine)
	: aiva::utils::AObject{}, aiva::utils::TObjectCacheable<ERvhCacheFlags>{ true }, aiva::layer1::IObjectEngineable{ engine }
{
	InitializeInternalResources();
}

aiva::layer1::ResourceViewHeap::ResourceViewHeap(EngineType const& engine, EDescriptorHeapType const heapType) : ResourceViewHeap(engine)
{
	HeapType(heapType);
}

aiva::layer1::ResourceViewHeap::~ResourceViewHeap()
{
	TerminateInternalResources();
}

aiva::layer1::EDescriptorHeapType aiva::layer1::ResourceViewHeap::HeapType() const
{
	return mHeapType;
}

void aiva::layer1::ResourceViewHeap::HeapType(EDescriptorHeapType const heapType)
{
	mHeapType = heapType;
	mViews = {};

	MarkCacheDataAsChanged();
}

aiva::layer1::ResourceViewHeap::ViewPtr aiva::layer1::ResourceViewHeap::GetView(std::string const& key) const
{
	auto const& viewIter = mViews.find(key);
	if (viewIter == mViews.end())
	{
		return {};
	}

	return viewIter->second;
}

void aiva::layer1::ResourceViewHeap::SetView(std::string const& key, ViewPtr const& value)
{
	aiva::utils::Asserts::CheckBool(!value || value->HeapType() == HeapType());

	auto const& previousIter = mViews.find(key);
	if (previousIter != mViews.end())
	{
		auto const& previousView = previousIter->second;
		aiva::utils::Asserts::CheckBool(previousView);

		if (previousView == value)
		{
			return;
		}

		previousView->OnMarkCacheDataAsChanged().disconnect(boost::bind(&ResourceViewHeap::View_OnMarkCacheDataAsChanged, this, boost::placeholders::_1));
		mViews.erase(previousIter);
	}

	if (!value)
	{
		MarkCacheDataAsChanged(ERvhCacheFlags::HeapBin);
		return;
	}

	auto const& currentIter = mViews.insert_or_assign(key, value).first;
	if (currentIter != mViews.end())
	{
		auto const& currentView = currentIter->second;
		aiva::utils::Asserts::CheckBool(currentView);

		currentView->OnMarkCacheDataAsChanged().connect(boost::bind(&ResourceViewHeap::View_OnMarkCacheDataAsChanged, this, boost::placeholders::_1));
	}

	MarkCacheDataAsChanged(ERvhCacheFlags::HeapBin);
}

aiva::layer1::ResourceViewHeap::ViewDict const& aiva::layer1::ResourceViewHeap::GetViews() const
{
	return mViews;
}

void aiva::layer1::ResourceViewHeap::View_OnMarkCacheDataAsChanged(EGrvCacheFlags const dirtyFlags)
{
	if (aiva::utils::EnumUtils::Has(dirtyFlags, EGrvCacheFlags::BufferPtr))
	{
		MarkCacheDataAsChanged(ERvhCacheFlags::HeapBin);
	}
}

winrt::com_ptr<ID3D12DescriptorHeap> const& aiva::layer1::ResourceViewHeap::InternalDescriptorHeap()
{
	FlushCacheDataChanges();
	return mInternalDescriptorHeap;
}

std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> const& aiva::layer1::ResourceViewHeap::InternalDescriptorHandles()
{
	FlushCacheDataChanges();
	return mInternalDescriptorHandles;
}

void aiva::layer1::ResourceViewHeap::InitializeInternalResources()
{
	FlushCacheDataExecutors().connect(boost::bind(&ResourceViewHeap::RefreshInternalResources, this, boost::placeholders::_1));
}

void aiva::layer1::ResourceViewHeap::TerminateInternalResources()
{
	FlushCacheDataExecutors().disconnect(boost::bind(&ResourceViewHeap::RefreshInternalResources, this, boost::placeholders::_1));
}

void aiva::layer1::ResourceViewHeap::RefreshInternalResources(ERvhCacheFlags const dirtyFlags)
{
	using namespace aiva::utils;

	auto needRefreshPtr = false;
	needRefreshPtr |= (EnumUtils::Has(dirtyFlags, ERvhCacheFlags::HeapPtr));
	needRefreshPtr |= (EnumUtils::Has(dirtyFlags, ERvhCacheFlags::HeapBin) && !mInternalDescriptorHeap);
	needRefreshPtr |= (EnumUtils::Has(dirtyFlags, ERvhCacheFlags::HeapBin) && mInternalDescriptorHeap && std::size(mViews) != mInternalDescriptorHeap->GetDesc().NumDescriptors);
	needRefreshPtr ? RefreshInternalDescriptorHeap() : [] {};

	auto needRefreshBin = false;
	needRefreshBin |= (needRefreshPtr);
	needRefreshBin |= (EnumUtils::Has(dirtyFlags, ERvhCacheFlags::HeapBin));
	needRefreshBin ? RefreshInternalDescriptorHandles() : [] {};
}

void aiva::layer1::ResourceViewHeap::RefreshInternalDescriptorHeap()
{
	mInternalDescriptorHeap = {};

	if (std::empty(mViews))
	{
		return;
	}

	auto const& device = Engine().GraphicHardware().Device();
	winrt::check_bool(device);

	auto heapDesc = D3D12_DESCRIPTOR_HEAP_DESC{};
	heapDesc.Type = ToInternalEnum(HeapType());
	heapDesc.NumDescriptors = std::size(mViews);
	heapDesc.Flags = SupportShaderAccess(HeapType()) ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
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

	auto const& device = Engine().GraphicHardware().Device();
	winrt::check_bool(device);

	auto const& views = mViews;
	aiva::utils::Asserts::CheckBool(!views.empty());

	auto const incrementSize = std::size_t{ device->GetDescriptorHandleIncrementSize(ToInternalEnum(HeapType())) };
	for (std::size_t i = {}; i < views.size(); i++)
	{
		auto const& iter = std::next(views.cbegin(), i);
		aiva::utils::Asserts::CheckBool(iter != views.end());

		auto const& view = iter->second;
		aiva::utils::Asserts::CheckBool(view);

		auto handle = mInternalDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += i * incrementSize;

		view->CreateDirectxView(handle);
		mInternalDescriptorHandles.emplace_back(handle);
	}
}

std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> aiva::layer1::ResourceViewHeap::InternalDescriptorHandle(std::string const& viewKey)
{
	auto const& iter = mViews.find(viewKey);
	if (iter == mViews.end())
	{
		return {};
	}

	auto const& viewIndex = std::distance(mViews.begin(), iter);
	aiva::utils::Asserts::CheckBool(viewIndex >= 0 & viewIndex < std::size(InternalDescriptorHandles()));

	return InternalDescriptorHandles().at(viewIndex);
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::ResourceViewHeap::PrepareBarriers(bool const active)
{
	auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

	auto const& resourceViews = mViews;
	std::for_each(std::cbegin(resourceViews), std::cend(resourceViews), [&barriers, active](auto const& resourceView)
	{
		aiva::utils::Asserts::CheckBool(resourceView.second);

		auto const& resourceBarriers = resourceView.second->CreateDirectxBarriers(active);
		std::copy(std::cbegin(resourceBarriers), std::cend(resourceBarriers), std::back_inserter(barriers));
	});

	return barriers;
}

void aiva::layer1::ResourceViewHeap::CopyPropertiesFrom(ResourceViewHeap const& source)
{
	HeapType(source.HeapType());

	auto keysToRemove = std::vector<std::string>{};
	for (auto const& view : mViews)
	{
		keysToRemove.emplace_back(view.first);
	}
	for (auto const& keyToRemove : keysToRemove)
	{
		SetView(keyToRemove, {});
	}

	for (auto const& view : source.mViews)
	{
		SetView(view.first, view.second);
	}
}
