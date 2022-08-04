#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EDescriptorHeapType : std::uint8_t
	{
		CbvSrvUav,
		Sampler,
		Rtv,
		Dsv,
	};

	// ----------------------------------------------------
	// DirectX

	EDescriptorHeapType FromInternalEnum(D3D12_DESCRIPTOR_HEAP_TYPE const heapType);

	D3D12_DESCRIPTOR_HEAP_TYPE ToInternalEnum(EDescriptorHeapType const heapType);
}
