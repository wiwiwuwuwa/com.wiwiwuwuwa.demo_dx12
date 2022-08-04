#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EResourceViewType : std::uint8_t
	{
		Cbv,
		Srv,
		Uav,
		Sampler,
		Rtv,
		Dsv,
	};

	EResourceViewType FromInternalDescriptorRangeType(D3D12_DESCRIPTOR_RANGE_TYPE const rangeType);

	D3D12_DESCRIPTOR_RANGE_TYPE ToInternalDescriptorRangeType(EResourceViewType const viewType);
}
