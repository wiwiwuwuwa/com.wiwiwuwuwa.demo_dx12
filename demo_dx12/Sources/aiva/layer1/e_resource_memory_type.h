#pragma once
#include <pch.h>

#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	enum class EResourceMemoryType : std::uint8_t
	{
		CpuToGpu,
		GpuOnly,
		GpuToCpu,
	};

	// ----------------------------------------------------
	// DirectX

	EResourceMemoryType FromInternalEnum(D3D12_HEAP_TYPE const heapType);

	D3D12_HEAP_TYPE ToInternalEnum(EResourceMemoryType const memoryType);
}
