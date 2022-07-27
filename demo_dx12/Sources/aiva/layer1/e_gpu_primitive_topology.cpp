#include <pch.h>
#include <aiva/layer1/e_gpu_primitive_topology.h>

#include <aiva/utils/asserts.h>

aiva::layer1::EGpuPrimitiveTopology aiva::layer1::FromInternalEnum(D3D_PRIMITIVE_TOPOLOGY const fillMode)
{
	switch (fillMode)
	{
	case D3D_PRIMITIVE_TOPOLOGY_UNDEFINED:
		return EGpuPrimitiveTopology::Undefined;

	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		return EGpuPrimitiveTopology::TriangleList;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D_PRIMITIVE_TOPOLOGY aiva::layer1::ToInternalEnum(EGpuPrimitiveTopology const fillMode)
{
	switch (fillMode)
	{
	case EGpuPrimitiveTopology::Undefined:
		return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

	case EGpuPrimitiveTopology::TriangleList:
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}
