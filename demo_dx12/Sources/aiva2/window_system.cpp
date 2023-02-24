#include <pch.h>
#include <aiva2/window_system.hpp>

#include <aiva2/asserts.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/window.hpp>

namespace aiva2
{
	window_system_t::window_system_t(engine_t& engine)
		: impl_type{ engine }
	{
		init_window();
	}

	window_system_t::~window_system_t()
	{
		shut_window();
	}

	void window_system_t::run() const
	{
		CoreApplication::Run(get_base_window());
	}

	auto window_system_t::get_base_window() const -> window_t&
	{
		asserts_t::check_true(m_base_window, "m_base_window is not valid");
		return *m_base_window;
	}

	auto window_system_t::get_core_window() const -> CoreWindow const&
	{
		return get_base_window().get_core_window();
	}

	auto window_system_t::get_on_window_init() -> event_action_type<>&
	{
		return m_on_window_init;
	}

	auto window_system_t::get_on_window_tick() -> event_action_type<>&
	{
		return m_on_window_tick;
	}

	auto window_system_t::get_on_window_shut() -> event_action_type<>&
	{
		return m_on_window_shut;
	}

	void window_system_t::init_window()
	{
		m_base_window = winrt::make_self<window_t>();
		asserts_t::check_true(m_base_window, "m_base_window is not valid");

		get_base_window().get_on_init().attach_listener(&this_type::window_system_when_window_on_init, this);
		get_base_window().get_on_tick().attach_listener(&this_type::window_system_when_window_on_tick, this);
		get_base_window().get_on_shut().attach_listener(&this_type::window_system_when_window_on_shut, this);
	}

	void window_system_t::shut_window()
	{
		asserts_t::check_true(m_base_window, "m_base_window is not valid");

		get_base_window().get_on_shut().detach_listener(&this_type::window_system_when_window_on_shut, this);
		get_base_window().get_on_tick().detach_listener(&this_type::window_system_when_window_on_tick, this);
		get_base_window().get_on_init().detach_listener(&this_type::window_system_when_window_on_init, this);
		m_base_window = {};
	}

	void window_system_t::window_system_when_window_on_init()
	{
		get_on_window_init().invoke();
	}

	void window_system_t::window_system_when_window_on_tick()
	{
		get_on_window_tick().invoke();
	}

	void window_system_t::window_system_when_window_on_shut()
	{
		get_on_window_shut().invoke();
	}
}
