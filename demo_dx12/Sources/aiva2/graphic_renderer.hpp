#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct graphic_renderer_t final : public implements_t<graphic_renderer_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		graphic_renderer_t(engine_t& engine);

		~graphic_renderer_t() override;

		// ------------------------------------------------

	public:
		auto get_screen_target() const->std::shared_ptr<render_target_t> const&;

	private:
		void init_screen_targets();
		
		void shut_screen_targets();

	private:
		std::vector<std::shared_ptr<render_target_t>> m_screen_targets{};

		// ------------------------------------------------
		
	public:
		auto get_on_render_world() -> event_action_type<>&;

	private:
		void init_on_render_world();

		void tick_on_render_world();

		void shut_on_render_world();

	private:
		event_action_type<> m_on_render_world{};

		// ------------------------------------------------
	};
}
