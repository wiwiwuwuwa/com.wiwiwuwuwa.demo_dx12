#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EGpuDescriptorHeapType : std::uint8_t
	{
		CbvSrvUav,
		Sampler,
		Rtv,
		Dsv,
	};

	EGpuDescriptorHeapType FromInternalEnum(D3D12_DESCRIPTOR_HEAP_TYPE const heapType);

	D3D12_DESCRIPTOR_HEAP_TYPE ToInternalEnum(EGpuDescriptorHeapType const heapType);
}
