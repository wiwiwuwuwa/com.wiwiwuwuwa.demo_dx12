#include <pch.h>
#include <aiva/layer1/shader_resource_descriptor.h>

#include <aiva/layer1/e_gpu_resource_view_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/utils/asserts.h>

aiva::layer1::ShaderResourceDescriptor::ShaderResourceDescriptor(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
	InitializeInternalResources();
}

aiva::layer1::ShaderResourceDescriptor::~ShaderResourceDescriptor()
{
	TerminateInternalResources();
	TerminateCacheUpdater();
}

aiva::utils::TCacheRefresh<aiva::layer1::ShaderResourceDescriptor::EDirtyFlags>& aiva::layer1::ShaderResourceDescriptor::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::ShaderResourceDescriptor::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<decltype(mCacheUpdater)::element_type>();
}

void aiva::layer1::ShaderResourceDescriptor::TerminateCacheUpdater()
{
	mCacheUpdater = {};
}

std::shared_ptr<aiva::layer1::IGpuResourceView> aiva::layer1::ShaderResourceDescriptor::ResourceView(std::string const& key) const
{
	auto const& resourceViewIter = mResourceViews.find(key);
	if (resourceViewIter == mResourceViews.end()) return {};

	return resourceViewIter->second;
}

aiva::layer1::ShaderResourceDescriptor& aiva::layer1::ShaderResourceDescriptor::ResourceView(std::string const& key, std::shared_ptr<IGpuResourceView> const& value)
{
	CacheUpdater().MarkAsChanged(EDirtyFlags::All);

	auto const& previousResourceIter = mResourceViews.find(key);
	if (previousResourceIter != mResourceViews.end())
	{
		auto const& previousResourceView = previousResourceIter->second;
		aiva::utils::Asserts::CheckBool(previousResourceView);

		previousResourceView->OnInternalResourceUpdated().disconnect(boost::bind(&ShaderResourceDescriptor::OnResourceViewUpdated, this));
		mResourceViews.erase(previousResourceIter);
	}

	if (!value)
	{
		return *this;
	}

	auto const& currentResourceIter = mResourceViews.insert_or_assign(key, value).first;
	if (currentResourceIter != mResourceViews.end())
	{
		auto const& currentResourceView = currentResourceIter->second;
		aiva::utils::Asserts::CheckBool(currentResourceView);

		currentResourceView->OnInternalResourceUpdated().connect(boost::bind(&ShaderResourceDescriptor::OnResourceViewUpdated, this));
	}

	return *this;
}

void aiva::layer1::ShaderResourceDescriptor::OnResourceViewUpdated()
{
	CacheUpdater().MarkAsChanged(EDirtyFlags::All);
}

void aiva::layer1::ShaderResourceDescriptor::InitializeInternalResources()
{
	CacheUpdater().OnFlushRequested().connect(boost::bind(&ShaderResourceDescriptor::RefreshInternalResources, this));
}

void aiva::layer1::ShaderResourceDescriptor::TerminateInternalResources()
{
	CacheUpdater().OnFlushRequested().disconnect(boost::bind(&ShaderResourceDescriptor::RefreshInternalResources, this));
}

void aiva::layer1::ShaderResourceDescriptor::RefreshInternalResources()
{
	RefreshDescriptorHeaps();
	RefreshRootSignature();

	OnInternalResourceUpdated()();
}

void aiva::layer1::ShaderResourceDescriptor::RefreshDescriptorHeaps()
{
	using ResourcePtr = std::shared_ptr<IGpuResourceView>;
	using HeapPtr = winrt::com_ptr<ID3D12DescriptorHeap>;

	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto resourceViewsPerName = std::vector<decltype(mResourceViews)::const_iterator>{};
	for (auto iter = mResourceViews.cbegin(); iter != mResourceViews.cend(); iter++) resourceViewsPerName.push_back(iter);
	std::sort(resourceViewsPerName.begin(), resourceViewsPerName.end(), [](auto const& a, auto const& b) { return a->first < b->first; });

	auto resourceViewsPerHeapType = std::unordered_map<EGpuDescriptorHeapType, std::vector<ResourcePtr>>{};
	for (auto const& pair : resourceViewsPerName)
	{
		auto const& resourceView = pair->second;
		aiva::utils::Asserts::CheckBool(resourceView);

		auto& resourceViews = resourceViewsPerHeapType[resourceView->DescriptorHeapType()];
		resourceViews.emplace_back(resourceView);
	}

	auto resourceHeapsPerHeapType = std::unordered_map<EGpuDescriptorHeapType, HeapPtr>{};
	for (auto const& pair : resourceViewsPerHeapType)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.Type = ToInternalEnum(pair.first);
		heapDesc.NumDescriptors = pair.second.size();
		heapDesc.Flags = pair.first == EGpuDescriptorHeapType::CbvSrvUav ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;

		HeapPtr resourceHeap{};
		winrt::check_hresult(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&resourceHeap)));

		winrt::check_bool(resourceHeap);
		resourceHeapsPerHeapType.insert_or_assign(pair.first, resourceHeap);

		auto const incrementSize = std::size_t{ device->GetDescriptorHandleIncrementSize(heapDesc.Type) };

		for (std::size_t i = 0; i < pair.second.size(); i++)
		{
			auto descHandle = resourceHeap->GetCPUDescriptorHandleForHeapStart();
			descHandle.ptr += i * incrementSize;

			auto const& resourceView = pair.second.at(i);
			aiva::utils::Asserts::CheckBool(resourceView);

			resourceView->CreateInternalResourceView(descHandle);
		}
	}

	mDescriptorHeaps = resourceHeapsPerHeapType;
}

void aiva::layer1::ShaderResourceDescriptor::RefreshRootSignature()
{
	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto rootParameters = std::vector<D3D12_ROOT_PARAMETER1>{};
	auto mainDescriptorRanges = std::vector<D3D12_DESCRIPTOR_RANGE1>{};
	auto samplerDescriptorRanges = std::vector<D3D12_DESCRIPTOR_RANGE1>{};

	auto const& mainDescriptorHeap = mDescriptorHeaps.find(EGpuDescriptorHeapType::CbvSrvUav);
	if (mainDescriptorHeap != mDescriptorHeaps.end())
	{
		auto registersCounters = std::unordered_map<EGpuResourceViewType, std::size_t>{};

		auto resourceViewsPerName = std::vector<decltype(mResourceViews)::const_iterator>{};
		for (auto iter = mResourceViews.cbegin(); iter != mResourceViews.cend(); iter++) { if (iter->second->DescriptorHeapType() == EGpuDescriptorHeapType::CbvSrvUav) { resourceViewsPerName.push_back(iter); }; };
		std::sort(resourceViewsPerName.begin(), resourceViewsPerName.end(), [](auto const& a, auto const& b) { return a->first < b->first; });
		
		for (const auto& pair : resourceViewsPerName)
		{
			auto& descriptorRange = mainDescriptorRanges.emplace_back();
			descriptorRange.RangeType = ToInternalDescriptorRangeType(pair->second->ResourceViewType());
			descriptorRange.NumDescriptors = 1;
			descriptorRange.BaseShaderRegister = registersCounters[pair->second->ResourceViewType()]++;
			descriptorRange.RegisterSpace = 0;
			descriptorRange.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
			descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		}

		auto& rootParameter = rootParameters.emplace_back();
		rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameter.DescriptorTable.NumDescriptorRanges = mainDescriptorRanges.size();
		rootParameter.DescriptorTable.pDescriptorRanges = mainDescriptorRanges.data();
		rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	}

	auto const& samplerDescriptorHeap = mDescriptorHeaps.find(EGpuDescriptorHeapType::Sampler);
	if (samplerDescriptorHeap != mDescriptorHeaps.end())
	{
		auto registersCounters = std::unordered_map<EGpuResourceViewType, std::size_t>{};

		auto resourceViewsPerName = std::vector<decltype(mResourceViews)::const_iterator>{};
		for (auto iter = mResourceViews.cbegin(); iter != mResourceViews.cend(); iter++) { if (iter->second->DescriptorHeapType() == EGpuDescriptorHeapType::CbvSrvUav) { resourceViewsPerName.push_back(iter); }; };
		std::sort(resourceViewsPerName.begin(), resourceViewsPerName.end(), [](auto const& a, auto const& b) { return a->first < b->first; });

		for (const auto& pair : resourceViewsPerName)
		{
			auto& descriptorRange = samplerDescriptorRanges.emplace_back();
			descriptorRange.RangeType = ToInternalDescriptorRangeType(pair->second->ResourceViewType());
			descriptorRange.NumDescriptors = 1;
			descriptorRange.BaseShaderRegister = registersCounters[pair->second->ResourceViewType()]++;
			descriptorRange.RegisterSpace = 0;
			descriptorRange.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
			descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		}

		auto& rootParameter = rootParameters.emplace_back();
		rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameter.DescriptorTable.NumDescriptorRanges = samplerDescriptorRanges.size();
		rootParameter.DescriptorTable.pDescriptorRanges = samplerDescriptorRanges.data();
		rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	}

	D3D12_ROOT_SIGNATURE_DESC1 signatureDesc{};
	signatureDesc.NumParameters = rootParameters.size();
	signatureDesc.pParameters = rootParameters.data();
	signatureDesc.NumStaticSamplers = 0;
	signatureDesc.pStaticSamplers = nullptr;
	signatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC versionedDesc{};
	versionedDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
	versionedDesc.Desc_1_1 = signatureDesc;

	winrt::com_ptr<ID3DBlob> signatureBlob{};
	winrt::com_ptr<ID3DBlob> errorMessages{};

	auto const serializationResult = D3D12SerializeVersionedRootSignature(&versionedDesc, signatureBlob.put(), errorMessages.put());
	if (FAILED(serializationResult))
	{
		winrt::check_bool(errorMessages);
		mEngine.LogToDebugConsole(static_cast<const char*>(errorMessages->GetBufferPointer()));
		winrt::throw_hresult(serializationResult);
	}

	winrt::com_ptr<ID3D12RootSignature> rootSignature{};
	winrt::check_hresult(device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));

	mRootSignature = rootSignature;
}

aiva::utils::EvAction& aiva::layer1::ShaderResourceDescriptor::OnInternalResourceUpdated()
{
	return mOnInternalResourceUpdated;
}
