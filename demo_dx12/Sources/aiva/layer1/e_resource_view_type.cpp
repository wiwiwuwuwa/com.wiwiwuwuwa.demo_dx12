#include <pch.h>
#include <aiva/layer1/e_resource_view_type.h>

#include <aiva/utils/asserts.h>

aiva::layer1::EResourceViewType aiva::layer1::FromInternalDescriptorRangeType(D3D12_DESCRIPTOR_RANGE_TYPE const rangeType)
{
	switch (rangeType)
	{
	case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
		return EResourceViewType::Srv;

	case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
		return EResourceViewType::Uav;

	case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
		return EResourceViewType::Cbv;

	case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
		return EResourceViewType::Sampler;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_DESCRIPTOR_RANGE_TYPE aiva::layer1::ToInternalDescriptorRangeType(EResourceViewType const viewType)
{
	switch (viewType)
	{
	case EResourceViewType::Cbv:
		return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;

	case EResourceViewType::Srv:
		return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;

	case EResourceViewType::Uav:
		return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;

	case EResourceViewType::Sampler:
		return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}
