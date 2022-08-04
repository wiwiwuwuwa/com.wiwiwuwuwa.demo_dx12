#include <pch.h>
#include <aiva/layer1/e_primitive_topology.h>

#include <aiva/utils/asserts.h>

aiva::layer1::EPrimitiveTopology aiva::layer1::FromInternalEnum(D3D_PRIMITIVE_TOPOLOGY const fillMode)
{
	switch (fillMode)
	{
	case D3D_PRIMITIVE_TOPOLOGY_UNDEFINED:
		return EPrimitiveTopology::Undefined;

	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		return EPrimitiveTopology::TriangleList;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}

D3D_PRIMITIVE_TOPOLOGY aiva::layer1::ToInternalEnum(EPrimitiveTopology const fillMode)
{
	switch (fillMode)
	{
	case EPrimitiveTopology::Undefined:
		return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

	case EPrimitiveTopology::TriangleList:
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	default:
		aiva::utils::Asserts::CheckBool(false);
		return {};
	}
}
