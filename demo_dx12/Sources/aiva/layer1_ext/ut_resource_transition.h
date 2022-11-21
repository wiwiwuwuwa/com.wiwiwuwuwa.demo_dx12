#pragma once
#include <pch.h>

#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct UtResourceTransition final
	{
	// ----------------------------------------------------
	// Main

	public:
		UtResourceTransition();

		UtResourceTransition(D3D12_RESOURCE_STATES const currentState, D3D12_RESOURCE_STATES const desiredState);

		UtResourceTransition(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const currentState, D3D12_RESOURCE_STATES const desiredState);

		~UtResourceTransition();

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_VAR_3(public, std::optional<std::size_t>, SubResource);

		M_OBJECT_FIELD_VAR_3(public, D3D12_RESOURCE_STATES, CurrentState);

		M_OBJECT_FIELD_VAR_3(public, D3D12_RESOURCE_STATES, DesiredState);
	};
}
