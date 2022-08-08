#include <pch.h>
#include <aiva/layer1/resource_view_heap.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/utils/asserts.h>

aiva::layer1::ResourceViewHeap::ResourceViewHeap(EngineType const& engine)
	: aiva::utils::AObject{}, aiva::utils::IObjectCacheable{ true }, aiva::layer1::IObjectEngineable{ engine }
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

	MarkAsChanged();
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

		previousView->OnMarkAsChanged().disconnect(boost::bind(&ResourceViewHeap::ExecuteMarkAsChangedForSelf, this));
		mViews.erase(previousIter);
		MarkAsChanged();
	}

	if (!value)
	{
		return;
	}

	auto const& currentIter = mViews.insert_or_assign(key, value).first;
	if (currentIter != mViews.end())
	{
		auto const& currentView = currentIter->second;
		aiva::utils::Asserts::CheckBool(currentView);

		currentView->OnMarkAsChanged().connect(boost::bind(&ResourceViewHeap::ExecuteMarkAsChangedForSelf, this));
		MarkAsChanged();
	}
}

aiva::layer1::ResourceViewHeap::ViewDict const& aiva::layer1::ResourceViewHeap::GetViews() const
{
	return mViews;
}

void aiva::layer1::ResourceViewHeap::ExecuteMarkAsChangedForSelf()
{
	MarkAsChanged();
}

winrt::com_ptr<ID3D12DescriptorHeap> const& aiva::layer1::ResourceViewHeap::InternalDescriptorHeap()
{
	FlushChanges();
	return mInternalDescriptorHeap;
}

std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> const& aiva::layer1::ResourceViewHeap::InternalDescriptorHandles()
{
	FlushChanges();
	return mInternalDescriptorHandles;
}

void aiva::layer1::ResourceViewHeap::InitializeInternalResources()
{
	FlushExecutors().connect(boost::bind(&ResourceViewHeap::RefreshInternalResources, this));
}

void aiva::layer1::ResourceViewHeap::TerminateInternalResources()
{
	FlushExecutors().disconnect(boost::bind(&ResourceViewHeap::RefreshInternalResources, this));
}

void aiva::layer1::ResourceViewHeap::RefreshInternalResources()
{
	ExecuteFlushForInternalResources();
	RefreshInternalDescriptorHeap();
	RefreshInternalDescriptorHandles();
}

void aiva::layer1::ResourceViewHeap::ExecuteFlushForInternalResources()
{
	for (auto const& view : mViews)
	{
		aiva::utils::Asserts::CheckBool(view.second);
		view.second->FlushChanges();
	}
}

void aiva::layer1::ResourceViewHeap::RefreshInternalDescriptorHeap()
{
	if (std::empty(mViews))
	{
		mInternalDescriptorHeap = {};
		return;
	}

	auto const& device = Engine().GraphicHardware().Device();
	winrt::check_bool(device);

	auto heapDesc = D3D12_DESCRIPTOR_HEAP_DESC{};
	heapDesc.Type = ToInternalEnum(HeapType());
	heapDesc.NumDescriptors = std::size(mViews);
	heapDesc.Flags = (HeapType() == EDescriptorHeapType::CbvSrvUav ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
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
	FlushChanges();

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
	FlushChanges();

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

	MarkAsChanged();
}
