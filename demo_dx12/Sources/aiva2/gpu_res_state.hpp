#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/struct.hpp>

namespace aiva2
{
	struct gpu_res_state_t final : public implements_t<gpu_res_state_t, struct_t>
	{
		// ------------------------------------------------

	public:
		gpu_res_state_t();

		gpu_res_state_t(D3D12_RESOURCE_STATES const state, size_t const count, bool const locked);

		~gpu_res_state_t() override;

	public:
		auto set_state(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index = {}) -> std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		auto set_state_for_one_subresource_vec(D3D12_RESOURCE_STATES const state, size_t const index) -> std::vector<D3D12_RESOURCE_BARRIER>;

		auto set_state_for_one_subresource_opt(D3D12_RESOURCE_STATES const state, size_t const index) -> std::optional<D3D12_RESOURCE_BARRIER>;

		auto set_state_for_all_subresources_vec(D3D12_RESOURCE_STATES const state) -> std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		std::vector<D3D12_RESOURCE_STATES> m_states{};

		bool m_locked{};

		// ------------------------------------------------
	};
}
