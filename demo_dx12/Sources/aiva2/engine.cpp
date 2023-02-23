#include <pch.h>
#include <aiva2/engine.hpp>

#include <aiva2/asserts.hpp>
#include <aiva2/window_system.hpp>

namespace aiva2
{
	engine_t::engine_t()
	{
		init_window_system();
	}

	engine_t::~engine_t()
	{
		shut_window_system();
	}

	void engine_t::run() const
	{
		get_window_system().run();
	}

	auto engine_t::get_window_system() const->window_system_t&
	{
		asserts_t::check_true(m_window_system, "m_window_system is not valid");
		return *m_window_system;
	}

	void engine_t::init_window_system()
	{
		m_window_system = std::make_unique<window_system_t>(*this);
		asserts_t::check_true(m_window_system, "m_window_system is not valid");
		
		get_window_system().get_on_window_init().attach_listener(&this_type::engine_when_window_system_on_window_init, this);
		get_window_system().get_on_window_tick().attach_listener(&this_type::engine_when_window_system_on_window_tick, this);
		get_window_system().get_on_window_shut().attach_listener(&this_type::engine_when_window_system_on_window_shut, this);
	}

	void engine_t::shut_window_system()
	{
		asserts_t::check_true(m_window_system, "m_window_system is not valid");

		get_window_system().get_on_window_shut().remove_listener(&this_type::engine_when_window_system_on_window_shut, this);
		get_window_system().get_on_window_tick().remove_listener(&this_type::engine_when_window_system_on_window_tick, this);
		get_window_system().get_on_window_init().remove_listener(&this_type::engine_when_window_system_on_window_init, this);
		m_window_system = {};
	}

	void engine_t::engine_when_window_system_on_window_init()
	{

	}

	void engine_t::engine_when_window_system_on_window_tick()
	{

	}

	void engine_t::engine_when_window_system_on_window_shut()
	{

	}
}
