#include <pch.h>
#include <aiva/layer1/e_gpu_resource_view_type.h>

#include <aiva/utils/asserts.h>

aiva::layer1::EGpuResourceViewType aiva::layer1::FromInternalDescriptorRangeType(D3D12_DESCRIPTOR_RANGE_TYPE const rangeType)
{
	switch (rangeType)
	{
	case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
		return EGpuResourceViewType::Srv;

	case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
		return EGpuResourceViewType::Uav;

	case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
		return EGpuResourceViewType::Cbv;

	case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
		return EGpuResourceViewType::Sampler;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_DESCRIPTOR_RANGE_TYPE aiva::layer1::ToInternalDescriptorRangeType(EGpuResourceViewType const viewType)
{
	switch (viewType)
	{
	case EGpuResourceViewType::Cbv:
		return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;

	case EGpuResourceViewType::Srv:
		return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;

	case EGpuResourceViewType::Uav:
		return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;

	case EGpuResourceViewType::Sampler:
		return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}
