#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/native/engine_object.hpp>

namespace aiva2::native
{
	struct graphic_pipeline_t final : public core::implements_t<graphic_pipeline_t, engine_object_t>
	{
		// ------------------------------------------------
		// graphic pipeline

	public:
		graphic_pipeline_t(engine_t& engine);

		~graphic_pipeline_t() override;

	public:
		auto get_on_populate_commands() -> event_action_type<>&;

	private:
		void init_graphic_pipeline();

		void tick_graphic_pipeline();

		void shut_graphic_pipeline();

	private:
		void wait_frame();

		void present_frame();

		void reset_command_list();

		void populate_command_list();

		void close_command_list();

		void execute_command_list();

		void execute_signal_for_frame();

	private:
		event_action_type<> m_on_populate_commands{};
	};
}
