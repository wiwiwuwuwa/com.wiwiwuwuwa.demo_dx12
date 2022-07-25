#include <pch.h>
#include <aiva/layer1/e_gpu_descriptor_heap_type.h>

#include <aiva/utils/asserts.h>

aiva::layer1::EGpuDescriptorHeapType aiva::layer1::FromInternalEnum(D3D12_DESCRIPTOR_HEAP_TYPE const heapType)
{
	switch (heapType)
	{
	case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
		return EGpuDescriptorHeapType::CbvSrvUav;

	case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
		return EGpuDescriptorHeapType::Sampler;

	case D3D12_DESCRIPTOR_HEAP_TYPE_RTV:
		return EGpuDescriptorHeapType::Rtv;

	case D3D12_DESCRIPTOR_HEAP_TYPE_DSV:
		return EGpuDescriptorHeapType::Dsv;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_DESCRIPTOR_HEAP_TYPE aiva::layer1::ToInternalEnum(EGpuDescriptorHeapType const heapType)
{
	switch (heapType)
	{
	case EGpuDescriptorHeapType::CbvSrvUav:
		return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	case EGpuDescriptorHeapType::Sampler:
		return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;

	case EGpuDescriptorHeapType::Rtv:
		return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	case EGpuDescriptorHeapType::Dsv:
		return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}
