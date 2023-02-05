#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/core/object.hpp>

namespace aiva2::native
{
	struct time_system_t final : public core::implements_t<time_system_t, core::object_t>
	{
		// ------------------------------------------------
		// time system

	public:
		time_system_t(engine_t& engine);

		~time_system_t() override;

	public:
		auto get_engine() const->engine_t&;

		auto get_on_update() -> event_action_type<>&;

		auto get_on_render() -> event_action_type<>&;

		auto get_tick() const->uint64_t;

		void tick();

	private:
		void init_time_system();

		void tick_time_system();

		void shut_time_system();

	private:
		engine_t& m_engine;

		event_action_type<> m_on_update{};

		event_action_type<> m_on_render{};

		uint64_t m_tick{};
	};
}
