#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/render_buffer_2d_base.hpp>
#include <aiva2/render_color_buffer_2d_info.hpp>
#include <aiva2/texture_2d_info.hpp>

namespace aiva2
{
	struct render_color_buffer_2d_t final : public implements_t<render_color_buffer_2d_t, render_buffer_2d_base_t>
	{
		// ------------------------------------------------

	public:
		render_color_buffer_2d_t(engine_t& engine, render_color_buffer_2d_info_t const& info, std::shared_ptr<texture_2d_t> const& resource);

		render_color_buffer_2d_t(engine_t& engine, render_color_buffer_2d_info_t const& info, texture_2d_info_t const& resource);

		render_color_buffer_2d_t(engine_t& engine, render_color_buffer_2d_info_t const& info, winrt::com_ptr<ID3D12Resource> const& resource);

		~render_color_buffer_2d_t() override;
		
		// ------------------------------------------------

	public:
		auto get_info() const->render_color_buffer_2d_info_t const&;

	private:
		render_color_buffer_2d_info_t m_info{};

		// ------------------------------------------------

	public:
		void create_in_handle(D3D12_CPU_DESCRIPTOR_HANDLE const& dst_handle) const;
		
		void set_state_for_transition() const;

		void set_state_for_uav() const;

		// ------------------------------------------------
	};
}
