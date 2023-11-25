#include <pch.h>
#include <aiva/layer1/e_descriptor_heap_type.h>

#include <aiva/utils/asserts.h>

aiva::layer1::EDescriptorHeapType aiva::layer1::FromInternalEnum(D3D12_DESCRIPTOR_HEAP_TYPE const heapType)
{
	switch (heapType)
	{
	case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		return EDescriptorHeapType::CbvSrvUav;

	case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		return EDescriptorHeapType::Sampler;

	case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
		return EDescriptorHeapType::Rtv;

	case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
		return EDescriptorHeapType::Dsv;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_DESCRIPTOR_HEAP_TYPE aiva::layer1::ToInternalEnum(EDescriptorHeapType const heapType)
{
	switch (heapType)
	{
	case EDescriptorHeapType::CbvSrvUav:
		return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	case EDescriptorHeapType::Sampler:
		return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;

	case EDescriptorHeapType::Rtv:
		return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	case EDescriptorHeapType::Dsv:
		return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

bool aiva::layer1::SupportShaderAccess(EDescriptorHeapType const heapType)
{
	switch (heapType)
	{
	case EDescriptorHeapType::CbvSrvUav:
		return true;

	case EDescriptorHeapType::Sampler:
		return true;

	case EDescriptorHeapType::Rtv:
		return false;

	case EDescriptorHeapType::Dsv:
		return false;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}
