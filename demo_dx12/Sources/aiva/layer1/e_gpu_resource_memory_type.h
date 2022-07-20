#pragma once
#include <pch.h>

#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	enum class EGpuResourceMemoryType : std::uint8_t
	{
		GpuOnly,
		CpuToGpu,
	};

	EGpuResourceMemoryType FromInternalEnum(D3D12_HEAP_TYPE const heapType);

	D3D12_HEAP_TYPE ToInternalEnum(EGpuResourceMemoryType const memoryType);
}
