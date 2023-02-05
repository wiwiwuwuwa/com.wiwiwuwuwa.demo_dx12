#include <pch.h>
#include <aiva2/native/time_system.hpp>

namespace aiva2::native
{
	time_system_t::time_system_t(engine_t& engine) : m_engine{ engine }
	{
		init_time_system();
	}

	time_system_t::~time_system_t()
	{
		shut_time_system();
	}

	auto time_system_t::get_engine() const -> engine_t&
	{
		return m_engine;
	}

	auto time_system_t::get_on_update() -> event_action_type<>&
	{
		return m_on_update;
	}

	auto time_system_t::get_on_render() -> event_action_type<>&
	{
		return m_on_render;
	}

	auto time_system_t::get_tick() const -> uint64_t
	{
		return m_tick;
	}

	void time_system_t::tick()
	{
		tick_time_system();
	}

	void time_system_t::init_time_system()
	{
		m_tick = {};
	}

	void time_system_t::tick_time_system()
	{
		get_on_update().broadcast();
		get_on_render().broadcast();
		m_tick++;
	}

	void time_system_t::shut_time_system()
	{
		m_tick = {};
	}
}
