#include <pch.h>
#include <aiva2/native/engine.hpp>

#include <aiva2/core/asserts.hpp>
#include <aiva2/native/graphic_hardware.hpp>
#include <aiva2/native/time_system.hpp>
#include <aiva2/native/window_system.hpp>

namespace aiva2::native
{
	engine_t::engine_t()
	{
		init_systems();
	}

	engine_t::~engine_t()
	{
		shut_systems();
	}

	void engine_t::run() const
	{
		get_window_system().run();
	}

	auto engine_t::get_window_system() const -> window_system_t&
	{
		core::asserts_t::check_true(m_window_system, "window system is not valid");
		return *m_window_system;
	}

	auto engine_t::get_time_system() const -> time_system_t&
	{
		core::asserts_t::check_true(m_time_system, "time system is not valid");
		return *m_time_system;
	}

	auto engine_t::get_graphic_hardware() const -> graphic_hardware_t&
	{
		core::asserts_t::check_true(m_graphic_hardware, "graphic hardware is not valid");
		return *m_graphic_hardware;
	}

	void engine_t::init_systems()
	{
		init_window_system();
	}

	void engine_t::shut_systems()
	{
		shut_window_system();
	}

	void engine_t::init_window_system()
	{
		m_window_system = std::make_unique<window_system_t>(*this);
		core::asserts_t::check_true(m_window_system, "window system is not valid");

		m_window_system->get_on_init().attach_listener(&this_type::systems_when_window_system_on_init, this);
		m_window_system->get_on_tick().attach_listener(&this_type::systems_when_window_system_on_tick, this);
		m_window_system->get_on_shut().attach_listener(&this_type::systems_when_window_system_on_shut, this);
	}

	void engine_t::shut_window_system()
	{
		core::asserts_t::check_true(m_window_system, "window system is not valid");

		m_window_system->get_on_shut().remove_listener(&this_type::systems_when_window_system_on_shut, this);
		m_window_system->get_on_tick().remove_listener(&this_type::systems_when_window_system_on_tick, this);
		m_window_system->get_on_init().remove_listener(&this_type::systems_when_window_system_on_init, this);
		
		m_window_system = {};
	}

	void engine_t::init_time_system()
	{
		m_time_system = std::make_unique<time_system_t>(*this);
		core::asserts_t::check_true(m_time_system, "time system is not valid");
	}

	void engine_t::tick_time_system()
	{
		core::asserts_t::check_true(m_time_system, "time system is not valid");
		m_time_system->tick();
	}

	void engine_t::shut_time_system()
	{
		core::asserts_t::check_true(m_time_system, "time system is not valid");
		m_time_system = {};
	}
	
	void engine_t::init_graphic_hardware()
	{
		m_graphic_hardware = std::make_unique<graphic_hardware_t>(*this);
		core::asserts_t::check_true(m_graphic_hardware, "graphic hardware is not valid");
	}

	void engine_t::shut_graphic_hardware()
	{
		core::asserts_t::check_true(m_graphic_hardware, "graphic hardware is not valid");
		m_graphic_hardware = {};
	}

	void engine_t::systems_when_window_system_on_init()
	{
		init_time_system();
		init_graphic_hardware();
	}

	void engine_t::systems_when_window_system_on_tick()
	{
		tick_time_system();
	}

	void engine_t::systems_when_window_system_on_shut()
	{
		shut_graphic_hardware();
		shut_time_system();
	}
}
