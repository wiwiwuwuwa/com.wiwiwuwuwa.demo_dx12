#include <pch.h>
#include <aiva/layer1/material_resource_descriptor.h>

#include <aiva/layer1/e_gpu_descriptor_heap_type.h>
#include <aiva/layer1/e_gpu_resource_view_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::MaterialResourceDescriptor::MaterialResourceDescriptor(Engine const& engine) : mEngine{ engine }
{
	InitializeCacheUpdater();
	InitializeResourceTable();
	InitializeInternalResources();
}

aiva::layer1::MaterialResourceDescriptor::~MaterialResourceDescriptor()
{
	TerminateInternalResources();
	TerminateResourceTable();
	TerminateCacheUpdater();
}

aiva::layer1::MaterialResourceDescriptor::CacheUpdaterType& aiva::layer1::MaterialResourceDescriptor::CacheUpdater() const
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	return *mCacheUpdater;
}

void aiva::layer1::MaterialResourceDescriptor::InitializeCacheUpdater()
{
	mCacheUpdater = std::make_unique<CacheUpdaterType>();
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
}

void aiva::layer1::MaterialResourceDescriptor::TerminateCacheUpdater()
{
	aiva::utils::Asserts::CheckBool(mCacheUpdater);
	mCacheUpdater = {};
}

aiva::layer1::ResourceViewTable& aiva::layer1::MaterialResourceDescriptor::ResourceTable() const
{
	aiva::utils::Asserts::CheckBool(mResourceTable);
	return *mResourceTable;
}

void aiva::layer1::MaterialResourceDescriptor::InitializeResourceTable()
{
	mResourceTable = ResourceViewTable::Create(mEngine);
	aiva::utils::Asserts::CheckBool(mResourceTable);

	mResourceTable->CacheUpdater().OnMarkAsChanged().connect(boost::bind(&MaterialResourceDescriptor::OnResourceTableMarkedAsChanged, this));
}

void aiva::layer1::MaterialResourceDescriptor::TerminateResourceTable()
{
	aiva::utils::Asserts::CheckBool(mResourceTable);

	mResourceTable->CacheUpdater().OnMarkAsChanged().disconnect(boost::bind(&MaterialResourceDescriptor::OnResourceTableMarkedAsChanged, this));
	mResourceTable = {};
}

void aiva::layer1::MaterialResourceDescriptor::OnResourceTableMarkedAsChanged()
{
	CacheUpdater().MarkAsChanged();
}

std::vector<winrt::com_ptr<ID3D12DescriptorHeap>> aiva::layer1::MaterialResourceDescriptor::InternalDescriptorHeaps() const
{
	CacheUpdater().FlushChanges();
	return ResourceTable().InternalResource();
}

winrt::com_ptr<ID3D12RootSignature> aiva::layer1::MaterialResourceDescriptor::InternalRootSignature() const
{
	CacheUpdater().FlushChanges();

	winrt::check_bool(mRootSignature);
	return mRootSignature;
}

void aiva::layer1::MaterialResourceDescriptor::InitializeInternalResources()
{
	CacheUpdater().FlushExecutors().connect(boost::bind(&MaterialResourceDescriptor::RefreshInternalResources, this));
}

void aiva::layer1::MaterialResourceDescriptor::TerminateInternalResources()
{
	CacheUpdater().FlushExecutors().disconnect(boost::bind(&MaterialResourceDescriptor::RefreshInternalResources, this));
}

void aiva::layer1::MaterialResourceDescriptor::RefreshInternalResources()
{
	RefreshRootSignature();
}

void aiva::layer1::MaterialResourceDescriptor::RefreshRootSignature()
{
	auto const& device = mEngine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto rootParameters = std::vector<D3D12_ROOT_PARAMETER1>{};
	auto mainDescriptorRanges = std::vector<D3D12_DESCRIPTOR_RANGE1>{};
	auto samplerDescriptorRanges = std::vector<D3D12_DESCRIPTOR_RANGE1>{};

	auto const& mainResourceHeap = ResourceTable().ResourceHeap(EGpuDescriptorHeapType::CbvSrvUav);
	if (mainResourceHeap)
	{
		auto registersCounters = std::unordered_map<EGpuResourceViewType, std::size_t>{};
		
		for (auto const& resourceView : mainResourceHeap->ResourceViews())
		{
			auto& descriptorRange = mainDescriptorRanges.emplace_back();
			descriptorRange.RangeType = ToInternalDescriptorRangeType(resourceView.second->ViewType());
			descriptorRange.NumDescriptors = 1;
			descriptorRange.BaseShaderRegister = registersCounters[resourceView.second->ViewType()]++;
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

	auto const& samplerResourceHeap = ResourceTable().ResourceHeap(EGpuDescriptorHeapType::Sampler);
	if (samplerResourceHeap)
	{
		auto registersCounters = std::unordered_map<EGpuResourceViewType, std::size_t>{};

		for (auto const& resourceView : samplerResourceHeap->ResourceViews())
		{
			auto& descriptorRange = samplerDescriptorRanges.emplace_back();
			descriptorRange.RangeType = ToInternalDescriptorRangeType(resourceView.second->ViewType());
			descriptorRange.NumDescriptors = 1;
			descriptorRange.BaseShaderRegister = registersCounters[resourceView.second->ViewType()]++;
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
