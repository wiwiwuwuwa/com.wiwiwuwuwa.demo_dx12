#include <pch.h>
#include <aiva2/time_system.hpp>

namespace aiva2
{
	time_system_t::time_system_t(engine_t& engine) : impl_type{ engine }
	{
		init_time_system();
	}

	time_system_t::~time_system_t()
	{
		shut_time_system();
	}

	auto time_system_t::get_on_pre_update() -> event_action_type<>&
	{
		return m_on_pre_update;
	}

	auto time_system_t::get_on_update() -> event_action_type<>&
	{
		return m_on_update;
	}

	auto time_system_t::get_on_post_update() -> event_action_type<>&
	{
		return m_on_post_update;
	}

	auto time_system_t::get_on_pre_render() -> event_action_type<>&
	{
		return m_on_pre_render;
	}

	auto time_system_t::get_on_render() -> event_action_type<>&
	{
		return m_on_render;
	}

	auto time_system_t::get_on_post_render() -> event_action_type<>&
	{
		return m_on_post_render;
	}

	auto time_system_t::get_tick() const -> size_t
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
		get_on_pre_update().invoke();
		get_on_update().invoke();
		get_on_post_update().invoke();
		get_on_pre_render().invoke();
		get_on_render().invoke();
		get_on_post_render().invoke();
		m_tick++;
	}

	void time_system_t::shut_time_system()
	{
		m_tick = {};
	}
}
