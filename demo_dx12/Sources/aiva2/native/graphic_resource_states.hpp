#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/core/struct.hpp>

namespace aiva2::native
{
	struct graphic_resource_states_t final : public core::implements_t<graphic_resource_states_t, core::struct_t>
	{
		// ------------------------------------------------
		// graphic resource states

	public:
		graphic_resource_states_t();

		graphic_resource_states_t(D3D12_RESOURCE_STATES const state, size_t const count, bool const locked);

		~graphic_resource_states_t() override;

	public:
		auto set_state(D3D12_RESOURCE_STATES const state, std::optional<size_t> const index = {}) -> std::vector<D3D12_RESOURCE_BARRIER>;

	private:		
		auto set_state_for_one_subresource_vec(D3D12_RESOURCE_STATES const state, size_t const index) -> std::vector<D3D12_RESOURCE_BARRIER>;
		
		auto set_state_for_one_subresource_opt(D3D12_RESOURCE_STATES const state, size_t const index) -> std::optional<D3D12_RESOURCE_BARRIER>;

		auto set_state_for_all_subresources_vec(D3D12_RESOURCE_STATES const state) -> std::vector<D3D12_RESOURCE_BARRIER>;

		auto set_state_for_all_subresources_using_single_barrier_vec(D3D12_RESOURCE_STATES const state) -> std::vector<D3D12_RESOURCE_BARRIER>;

		auto set_state_for_all_subresources_using_single_barrier_opt(D3D12_RESOURCE_STATES const state) -> std::optional<D3D12_RESOURCE_BARRIER>;

		auto set_state_for_all_subresources_using_multiple_barriers_vec(D3D12_RESOURCE_STATES const state) -> std::vector<D3D12_RESOURCE_BARRIER>;

	private:
		std::vector<D3D12_RESOURCE_STATES> m_states{};

		bool m_locked{};
	};
}
