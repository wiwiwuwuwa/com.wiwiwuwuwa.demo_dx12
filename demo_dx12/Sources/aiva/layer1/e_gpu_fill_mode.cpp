#include <pch.h>
#include <aiva/layer1/e_gpu_fill_mode.h>

#include <aiva/utils/asserts.h>

aiva::layer1::EGpuFillMode aiva::layer1::FromInternalEnum(D3D12_FILL_MODE const fillMode)
{
	switch (fillMode)
	{
	case D3D12_FILL_MODE_WIREFRAME:
		return EGpuFillMode::Wireframe;

	case D3D12_FILL_MODE_SOLID:
		return EGpuFillMode::Solid;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D12_FILL_MODE aiva::layer1::ToInternalEnum(EGpuFillMode const fillMode)
{
	switch (fillMode)
	{
	case EGpuFillMode::Wireframe:
		return D3D12_FILL_MODE_WIREFRAME;

	case EGpuFillMode::Solid:
		return D3D12_FILL_MODE_SOLID;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}
