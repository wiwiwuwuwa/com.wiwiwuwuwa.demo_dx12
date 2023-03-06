#include <pch.h>
#include <aiva2/engine.hpp>

#include <aiva2/asserts.hpp>
#include <aiva2/graphic_executor.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/graphic_pipeline.hpp>
#include <aiva2/time_system.hpp>
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

		get_window_system().get_on_window_shut().detach_listener(&this_type::engine_when_window_system_on_window_shut, this);
		get_window_system().get_on_window_tick().detach_listener(&this_type::engine_when_window_system_on_window_tick, this);
		get_window_system().get_on_window_init().detach_listener(&this_type::engine_when_window_system_on_window_init, this);
		m_window_system = {};
	}

	void engine_t::engine_when_window_system_on_window_init()
	{
		init_time_system();
		init_graphic_hardware();
		init_graphic_pipeline();
		init_graphic_executor();
	}

	void engine_t::engine_when_window_system_on_window_tick()
	{
		tick_time_system();
	}

	void engine_t::engine_when_window_system_on_window_shut()
	{
		shut_graphic_executor();
		shut_graphic_pipeline();
		shut_graphic_hardware();
		shut_time_system();
	}

	auto engine_t::get_time_system() const->time_system_t&
	{
		asserts_t::check_true(m_time_system, "m_time_system is not valid");
		return *m_time_system;
	}

	void engine_t::init_time_system()
	{
		m_time_system = std::make_unique<time_system_t>(*this);
		asserts_t::check_true(m_time_system, "m_time_system is not valid");
	}

	void engine_t::tick_time_system()
	{
		get_time_system().tick();
	}

	void engine_t::shut_time_system()
	{
		asserts_t::check_true(m_time_system, "m_time_system is not valid");
		m_time_system = {};
	}

	auto engine_t::get_graphic_hardware() const->graphic_hardware_t&
	{
		asserts_t::check_true(m_graphic_hardware, "m_graphic_hardware is not valid");
		return *m_graphic_hardware;
	}

	void engine_t::init_graphic_hardware()
	{
		m_graphic_hardware = std::make_unique<graphic_hardware_t>(*this);
		asserts_t::check_true(m_graphic_hardware, "m_graphic_hardware is not valid");
	}

	void engine_t::shut_graphic_hardware()
	{
		asserts_t::check_true(m_graphic_hardware, "m_graphic_hardware is not valid");
		m_graphic_hardware = {};
	}

	auto engine_t::get_graphic_pipeline() const->graphic_pipeline_t&
	{
		asserts_t::check_true(m_graphic_pipeline, "m_graphic_pipeline is not valid");
		return *m_graphic_pipeline;
	}
	
	void engine_t::init_graphic_pipeline()
	{
		m_graphic_pipeline = std::make_unique<graphic_pipeline_t>(*this);
		asserts_t::check_true(m_graphic_pipeline, "m_graphic_pipeline is not valid");
	}
	
	void engine_t::shut_graphic_pipeline()
	{
		asserts_t::check_true(m_graphic_pipeline, "m_graphic_pipeline is not valid");
		m_graphic_pipeline = {};
	}

	auto engine_t::get_graphic_executor() const->graphic_executor_t&
	{
		asserts_t::check_true(m_graphic_executor, "m_graphic_executor is not valid");
		return *m_graphic_executor;
	}

	void engine_t::init_graphic_executor()
	{
		m_graphic_executor = std::make_unique<graphic_executor_t>(*this);
		asserts_t::check_true(m_graphic_executor, "m_graphic_executor is not valid");
	}
	
	void engine_t::shut_graphic_executor()
	{
		asserts_t::check_true(m_graphic_executor, "m_graphic_executor is not valid");
		m_graphic_executor = {};
	}
}