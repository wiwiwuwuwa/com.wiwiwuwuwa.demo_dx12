#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/render_texture_2d_base.hpp>
#include <aiva2/render_depth_texture_2d_info.hpp>
#include <aiva2/texture_2d_info.hpp>

namespace aiva2
{
	struct render_depth_texture_2d_t final : public implements_t<render_depth_texture_2d_t, render_texture_2d_base_t>
	{
		// ------------------------------------------------

	public:
		render_depth_texture_2d_t(engine_t& engine, std::shared_ptr<texture_2d_t> const& resource, render_depth_texture_2d_info_t const& info = {});

		~render_depth_texture_2d_t() override;

	public:
		template <typename t_resource>
		render_depth_texture_2d_t(engine_t& engine, t_resource const& resource, render_depth_texture_2d_info_t const& info = {});

		// ------------------------------------------------

	public:
		void init_state_for_transition() const override;

		void shut_state_for_transition() const override;

		void init_state_for_uav() const override;

	private:
		void init_descriptor_heap();

		// ------------------------------------------------

	public:
		auto get_info() const->render_depth_texture_2d_info_t const&;

	private:
		render_depth_texture_2d_info_t m_info{};
		
		// ------------------------------------------------
	};
}

// --------------------------------------------------------

namespace aiva2
{
	template <typename t_resource>
	render_depth_texture_2d_t::render_depth_texture_2d_t(engine_t& engine, t_resource const& resource, render_depth_texture_2d_info_t const& info /*= {}*/)
		: render_depth_texture_2d_t
		{
			/*engine*/ engine,
			/*resource*/ std::make_shared<texture_2d_t>(engine, resource),
			/*info*/ info
		}
	{

	}
}
