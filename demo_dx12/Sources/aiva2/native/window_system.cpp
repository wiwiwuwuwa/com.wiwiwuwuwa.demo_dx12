#include <pch.h>
#include <aiva2/native/window_system.hpp>

#include <aiva2/core/asserts.hpp>
#include <aiva2/native/window.hpp>
#include <aiva2/native/engine.hpp>

namespace aiva2::native
{
	window_system_t::window_system_t(engine_t& engine) : impl_type{ engine }
	{
		init_window();
		init_window_system();
	}

	window_system_t::~window_system_t()
	{
		shut_window_system();
		shut_window();
	}

	auto window_system_t::get_on_init() -> event_action_type<>&
	{
		return m_on_init;
	}

	auto window_system_t::get_on_tick() -> event_action_type<>&
	{
		return m_on_tick;
	}

	auto window_system_t::get_on_shut() -> event_action_type<>&
	{
		return m_on_shut;
	}

	void window_system_t::run() const
	{
		CoreApplication::Run(get_base_window());
	}

	void window_system_t::init_window_system()
	{
		get_base_window().get_on_init().attach_listener(&this_type::window_system_when_window_on_init, this);
		get_base_window().get_on_tick().attach_listener(&this_type::window_system_when_window_on_tick, this);
		get_base_window().get_on_shut().attach_listener(&this_type::window_system_when_window_on_shut, this);
	}

	void window_system_t::shut_window_system()
	{
		get_base_window().get_on_shut().remove_listener(&this_type::window_system_when_window_on_shut, this);
		get_base_window().get_on_tick().remove_listener(&this_type::window_system_when_window_on_tick, this);
		get_base_window().get_on_init().remove_listener(&this_type::window_system_when_window_on_init, this);
	}

	void window_system_t::window_system_when_window_on_init()
	{
		get_on_init().broadcast();
	}

	void window_system_t::window_system_when_window_on_tick()
	{
		get_on_tick().broadcast();
	}

	void window_system_t::window_system_when_window_on_shut()
	{
		get_on_shut().broadcast();
	}

	auto window_system_t::get_base_window() const->window_t&
	{
		core::asserts_t::check_true(m_base_window, "base window is not valid");
		return *m_base_window;
	}

	auto window_system_t::get_core_window() const->CoreWindow const&
	{
		return get_base_window().get_core_window();
	}

	void window_system_t::init_window()
	{
		m_base_window = winrt::make_self<window_t>();
		core::asserts_t::check_true(m_base_window, "base window is not valid");
	}

	void window_system_t::shut_window()
	{
		core::asserts_t::check_true(m_base_window, "base window is not valid");
		m_base_window = {};
	}
}
