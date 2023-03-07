#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/gpu_res_state.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct gpu_res_base_t : public implements_t<gpu_res_base_t, engine_object_t>
	{
		// ------------------------------------------------

	protected:
		gpu_res_base_t(engine_t& engine);

		~gpu_res_base_t() override;

		// ------------------------------------------------
		
	protected:
		winrt::com_ptr<ID3D12Resource> m_resource{};

		// ------------------------------------------------

	public:
		auto set_state_for_transition(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index = {}) -> std::vector<D3D12_RESOURCE_BARRIER>;

		auto set_state_for_uav() -> D3D12_RESOURCE_BARRIER;

	protected:		
		gpu_res_state_t m_states{};

		// ------------------------------------------------
	};
}
