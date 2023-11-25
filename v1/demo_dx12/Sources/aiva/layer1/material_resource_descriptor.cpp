#include <pch.h>
#include <aiva/layer1/material_resource_descriptor.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/e_resource_view_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/enum_utils.h>
#include <aiva/utils/logger.h>
#include <aiva/utils/object_utils.h>
#include <aiva/utils/t_cache_updater.h>

aiva::layer1::MaterialResourceDescriptor::MaterialResourceDescriptor(EngineType const& engine) : aiva::utils::AObject{}, aiva::utils::IObjectCacheable{ true }, IObjectEngineable{ engine }
{
	InitializeResourceTable();
	InitializeInternalResources();
}

aiva::layer1::MaterialResourceDescriptor::~MaterialResourceDescriptor()
{
	TerminateInternalResources();
	TerminateResourceTable();
}

aiva::layer1::ResourceViewTable& aiva::layer1::MaterialResourceDescriptor::ResourceTable() const
{
	aiva::utils::Asserts::CheckBool(mResourceTable);
	return *mResourceTable;
}

void aiva::layer1::MaterialResourceDescriptor::InitializeResourceTable()
{
	mResourceTable = aiva::utils::NewObject<ResourceViewTableType>(Engine());
	aiva::utils::Asserts::CheckBool(mResourceTable);

	mResourceTable->OnCacheDataChanged().connect(boost::bind(&MaterialResourceDescriptor::OnResourceTableMarkedAsChanged, this, boost::placeholders::_1));
}

void aiva::layer1::MaterialResourceDescriptor::TerminateResourceTable()
{
	aiva::utils::Asserts::CheckBool(mResourceTable);

	mResourceTable->OnCacheDataChanged().disconnect(boost::bind(&MaterialResourceDescriptor::OnResourceTableMarkedAsChanged, this, boost::placeholders::_1));
	mResourceTable = {};
}

void aiva::layer1::MaterialResourceDescriptor::OnResourceTableMarkedAsChanged(ERvtCacheFlags const dirtyFlags)
{
	auto needMarkAsChanged = aiva::utils::EnumUtils::Has(dirtyFlags, ERvtCacheFlags::TablePtr);
	needMarkAsChanged ? MarkCacheDataAsChanged() : [] {};
}

std::vector<winrt::com_ptr<ID3D12DescriptorHeap>> aiva::layer1::MaterialResourceDescriptor::InternalDescriptorHeaps()
{
	return ResourceTable().InternalResource();
}

winrt::com_ptr<ID3D12RootSignature> aiva::layer1::MaterialResourceDescriptor::InternalRootSignature()
{
	FlushCacheDataChanges();

	winrt::check_bool(mRootSignature);
	return mRootSignature;
}

void aiva::layer1::MaterialResourceDescriptor::InitializeInternalResources()
{
	FlushCacheDataExecutors().connect(boost::bind(&MaterialResourceDescriptor::RefreshInternalResources, this));
}

void aiva::layer1::MaterialResourceDescriptor::TerminateInternalResources()
{
	FlushCacheDataExecutors().disconnect(boost::bind(&MaterialResourceDescriptor::RefreshInternalResources, this));
}

void aiva::layer1::MaterialResourceDescriptor::RefreshInternalResources()
{
	RefreshRootSignature();
}

void aiva::layer1::MaterialResourceDescriptor::RefreshRootSignature()
{
	auto const& device = Engine().GraphicHardware().Device();
	winrt::check_bool(device);

	auto rootParameters = std::vector<D3D12_ROOT_PARAMETER1>{};
	auto mainDescriptorRanges = std::vector<D3D12_DESCRIPTOR_RANGE1>{};
	auto samplerDescriptorRanges = std::vector<D3D12_DESCRIPTOR_RANGE1>{};

	auto const& mainResourceHeap = ResourceTable().GetResourceHeap(EDescriptorHeapType::CbvSrvUav);
	if (mainResourceHeap)
	{
		auto registersCounters = std::unordered_map<EResourceViewType, std::size_t>{};
		
		for (auto const& resourceView : mainResourceHeap->GetViews())
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

	auto const& samplerResourceHeap = ResourceTable().GetResourceHeap(EDescriptorHeapType::Sampler);
	if (samplerResourceHeap)
	{
		auto registersCounters = std::unordered_map<EResourceViewType, std::size_t>{};

		for (auto const& resourceView : samplerResourceHeap->GetViews())
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
		aiva::utils::Logger::LogToDebugConsole(static_cast<const char*>(errorMessages->GetBufferPointer()));
		winrt::throw_hresult(serializationResult);
	}

	winrt::com_ptr<ID3D12RootSignature> rootSignature{};
	winrt::check_hresult(device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));

	mRootSignature = rootSignature;
}

std::vector<D3D12_RESOURCE_BARRIER> aiva::layer1::MaterialResourceDescriptor::PrepareBarriers(bool const active) const
{
	return ResourceTable().PrepareBarriers(active);
}

void aiva::layer1::MaterialResourceDescriptor::CopyPropertiesFrom(MaterialResourceDescriptor const& source)
{
	ResourceTable().CopyPropertiesFrom(source.ResourceTable());
	MarkCacheDataAsChanged();
}
