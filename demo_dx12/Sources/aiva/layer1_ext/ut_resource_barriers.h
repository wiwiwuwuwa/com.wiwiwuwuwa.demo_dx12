#pragma once
#include <pch.h>

#include <aiva/layer1_ext/ut_resource_transition.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct UtResourceBarriers final
	{
	// ----------------------------------------------------
	// Main

	public:
		UtResourceBarriers(D3D12_RESOURCE_STATES const initialState = D3D12_RESOURCE_STATE_COMMON);

		~UtResourceBarriers();

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_VAR_3(private, D3D12_RESOURCE_STATES, MainState);

		M_OBJECT_FIELD_VAR_3(private, bool, IsConstant);

		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::unordered_map<std::size_t, D3D12_RESOURCE_STATES>), SubStates);

	public:
		std::vector<UtResourceTransition> Transite(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const desiredState);

	private:
		std::vector<UtResourceTransition> TransiteUncheked(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const desiredState);

	private:
		std::optional<UtResourceTransition> TransiteMainResource(D3D12_RESOURCE_STATES const desiredState);

		std::optional<UtResourceTransition> TransiteSubResource(std::size_t const subResource, D3D12_RESOURCE_STATES const desiredState);

	private:
		D3D12_RESOURCE_STATES GetSubResourceState(std::size_t const subResource) const;

		void SetSubResourceState(std::size_t const subResource, D3D12_RESOURCE_STATES const desiredState);
	};
}
