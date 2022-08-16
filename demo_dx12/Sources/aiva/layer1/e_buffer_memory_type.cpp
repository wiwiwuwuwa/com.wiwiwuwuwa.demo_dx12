#include <pch.h>
#include <aiva/layer1/e_buffer_memory_type.h>

#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	EBufferMemoryType FromInternalEnum(D3D12_HEAP_TYPE const heapType)
	{
		switch (heapType)
		{
		case D3D12_HEAP_TYPE_DEFAULT:
			return EBufferMemoryType::GpuOnly;

		case D3D12_HEAP_TYPE_UPLOAD:
			return EBufferMemoryType::CpuToGpu;

		case D3D12_HEAP_TYPE_READBACK:
			return EBufferMemoryType::GpuToCpu;

		default:
			Asserts::CheckBool(false, "Unsupported D3D12_HEAP_TYPE enum value");
			return {};
		}
	}

	D3D12_HEAP_TYPE ToInternalEnum(EBufferMemoryType const memoryType)
	{
		switch (memoryType)
		{
		case EBufferMemoryType::CpuToGpu:
			return D3D12_HEAP_TYPE_UPLOAD;

		case EBufferMemoryType::GpuOnly:
			return D3D12_HEAP_TYPE_DEFAULT;

		case EBufferMemoryType::GpuToCpu:
			return D3D12_HEAP_TYPE_READBACK;

		default:
			Asserts::CheckBool(false, "Unsupported EBufferMemoryType enum value");
			return {};
		}
	}
}
