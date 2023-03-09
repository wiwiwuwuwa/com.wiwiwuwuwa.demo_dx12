#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/render_buffer_2d_base.hpp>
#include <aiva2/render_depth_buffer_2d_info.hpp>
#include <aiva2/texture_2d_info.hpp>

namespace aiva2
{
	struct render_depth_buffer_2d_t final : public implements_t<render_depth_buffer_2d_t, render_buffer_2d_base_t>
	{
		// ------------------------------------------------

	public:
		render_depth_buffer_2d_t(engine_t& engine, render_depth_buffer_2d_info_t const& info, std::shared_ptr<texture_2d_t> const& resource);

		template <typename... t_resource_args>
		render_depth_buffer_2d_t(engine_t& engine, render_depth_buffer_2d_info_t const& info, t_resource_args&&... resource_args);

		~render_depth_buffer_2d_t() override;

	public:
		void set_state_for_transition() const;

		void set_state_for_uav() const;

	private:
		void init_descriptor_heap();

		// ------------------------------------------------

	public:
		auto get_info() const->render_depth_buffer_2d_info_t const&;

	private:
		render_depth_buffer_2d_info_t m_info{};
		
		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename... t_resource_args>
	render_depth_buffer_2d_t::render_depth_buffer_2d_t(engine_t& engine, render_depth_buffer_2d_info_t const& info, t_resource_args&&... resource_args)
		: render_depth_buffer_2d_t
		{
			/*engine*/ engine,
			/*info*/ info,
			/*resource*/ std::make_shared<texture_2d_t>(engine, std::forward<t_resource_args>(resource_args)...)
		}
	{

	}
}
