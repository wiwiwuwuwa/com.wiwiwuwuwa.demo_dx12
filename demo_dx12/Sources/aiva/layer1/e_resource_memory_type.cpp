#include <pch.h>
#include <aiva/layer1/e_resource_memory_type.h>

aiva::layer1::EResourceMemoryType aiva::layer1::FromInternalEnum(D3D12_HEAP_TYPE const heapType)
{
	switch (heapType)
	{
	case D3D12_HEAP_TYPE_DEFAULT:
		return EResourceMemoryType::GpuOnly;

	case D3D12_HEAP_TYPE_UPLOAD:
		return EResourceMemoryType::CpuToGpu;

	case D3D12_HEAP_TYPE_READBACK:
		return EResourceMemoryType::GpuToCpu;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_HEAP_TYPE aiva::layer1::ToInternalEnum(EResourceMemoryType const memoryType)
{
	switch (memoryType)
	{
	case EResourceMemoryType::CpuToGpu:
		return D3D12_HEAP_TYPE_UPLOAD;

	case EResourceMemoryType::GpuOnly:
		return D3D12_HEAP_TYPE_DEFAULT;

	case EResourceMemoryType::GpuToCpu:
		return D3D12_HEAP_TYPE_READBACK;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}
