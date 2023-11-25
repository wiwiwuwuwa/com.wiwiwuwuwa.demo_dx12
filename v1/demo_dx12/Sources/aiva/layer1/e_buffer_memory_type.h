#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EBufferMemoryType
	{
		CpuToGpu,
		GpuOnly,
		GpuToCpu,
	};

	// ----------------------------------------------------
	// DirectX

	EBufferMemoryType FromInternalEnum(D3D12_HEAP_TYPE const heapType);

	D3D12_HEAP_TYPE ToInternalEnum(EBufferMemoryType const memoryType);
}
