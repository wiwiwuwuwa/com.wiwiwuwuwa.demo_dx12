#include <pch.h>
#include <aiva/layer1/e_gpu_resource_memory_type.h>

aiva::layer1::EGpuResourceMemoryType aiva::layer1::FromInternalEnum(D3D12_HEAP_TYPE const heapType)
{
	switch (heapType)
	{
	case D3D12_HEAP_TYPE_DEFAULT:
		return EGpuResourceMemoryType::GpuOnly;

	case D3D12_HEAP_TYPE_UPLOAD:
		return EGpuResourceMemoryType::CpuToGpu;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_HEAP_TYPE aiva::layer1::ToInternalEnum(EGpuResourceMemoryType const memoryType)
{
	switch (memoryType)
	{
	case EGpuResourceMemoryType::GpuOnly:
		return D3D12_HEAP_TYPE_DEFAULT;

	case EGpuResourceMemoryType::CpuToGpu:
		return D3D12_HEAP_TYPE_UPLOAD;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}
