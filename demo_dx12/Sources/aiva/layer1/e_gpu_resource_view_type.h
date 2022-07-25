#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EGpuResourceViewType : std::uint8_t
	{
		Cbv,
		Srv,
		Uav,
		Sampler,
	};

	EGpuResourceViewType FromInternalDescriptorRangeType(D3D12_DESCRIPTOR_RANGE_TYPE const rangeType);

	D3D12_DESCRIPTOR_RANGE_TYPE ToInternalDescriptorRangeType(EGpuResourceViewType const viewType);
}
