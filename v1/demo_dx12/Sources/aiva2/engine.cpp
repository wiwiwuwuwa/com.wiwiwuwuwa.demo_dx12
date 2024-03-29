#include <pch.h>
#include <aiva2/engine.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/deffered_renderer.hpp>
#include <aiva2/graphic_executor.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/graphic_pipeline.hpp>
#include <aiva2/graphic_renderer.hpp>
#include <aiva2/resource_system.hpp>
#include <aiva2/scene_system.hpp>
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
		assert_t::check_bool(m_window_system, "m_window_system is not valid");
		return *m_window_system;
	}

	void engine_t::init_window_system()
	{
		m_window_system = std::make_unique<window_system_t>(*this);
		assert_t::check_bool(m_window_system, "m_window_system is not valid");

		get_window_system().get_on_window_init().attach_listener(&this_type::engine_when_window_system_on_window_init, this);
		get_window_system().get_on_window_tick().attach_listener(&this_type::engine_when_window_system_on_window_tick, this);
		get_window_system().get_on_window_shut().attach_listener(&this_type::engine_when_window_system_on_window_shut, this);
	}

	void engine_t::shut_window_system()
	{
		assert_t::check_bool(m_window_system, "m_window_system is not valid");

		get_window_system().get_on_window_shut().detach_listener(&this_type::engine_when_window_system_on_window_shut, this);
		get_window_system().get_on_window_tick().detach_listener(&this_type::engine_when_window_system_on_window_tick, this);
		get_window_system().get_on_window_init().detach_listener(&this_type::engine_when_window_system_on_window_init, this);
		m_window_system = {};
	}

	void engine_t::engine_when_window_system_on_window_init()
	{
		init_resource_system();
		init_time_system();
		init_graphic_hardware();
		init_graphic_pipeline();
		init_graphic_executor();
		init_graphic_renderer();
		init_scene_system();
		init_deffered_renderer();
	}

	void engine_t::engine_when_window_system_on_window_tick()
	{
		tick_time_system();
	}

	void engine_t::engine_when_window_system_on_window_shut()
	{
		shut_deffered_renderer();
		shut_scene_system();
		shut_graphic_renderer();
		shut_graphic_executor();
		shut_graphic_pipeline();
		shut_graphic_hardware();
		shut_time_system();
		shut_resource_system();
	}

	auto engine_t::get_resource_system() const->resource_system_t&
	{
		assert_t::check_bool(m_resource_system, "m_resource_system is not valid");
		return *m_resource_system;
	}

	void engine_t::init_resource_system()
	{
		m_resource_system = std::make_unique<resource_system_t>(*this);
		assert_t::check_bool(m_resource_system, "m_resource_system is not valid");
	}

	void engine_t::shut_resource_system()
	{
		assert_t::check_bool(m_resource_system, "m_resource_system is not valid");
		m_resource_system = {};
	}

	auto engine_t::get_time_system() const->time_system_t&
	{
		assert_t::check_bool(m_time_system, "m_time_system is not valid");
		return *m_time_system;
	}

	void engine_t::init_time_system()
	{
		m_time_system = std::make_unique<time_system_t>(*this);
		assert_t::check_bool(m_time_system, "m_time_system is not valid");
	}

	void engine_t::tick_time_system()
	{
		get_time_system().tick();
	}

	void engine_t::shut_time_system()
	{
		assert_t::check_bool(m_time_system, "m_time_system is not valid");
		m_time_system = {};
	}

	auto engine_t::get_graphic_hardware() const->graphic_hardware_t&
	{
		assert_t::check_bool(m_graphic_hardware, "m_graphic_hardware is not valid");
		return *m_graphic_hardware;
	}

	void engine_t::init_graphic_hardware()
	{
		m_graphic_hardware = std::make_unique<graphic_hardware_t>(*this);
		assert_t::check_bool(m_graphic_hardware, "m_graphic_hardware is not valid");
	}

	void engine_t::shut_graphic_hardware()
	{
		assert_t::check_bool(m_graphic_hardware, "m_graphic_hardware is not valid");
		m_graphic_hardware = {};
	}

	auto engine_t::get_graphic_pipeline() const->graphic_pipeline_t&
	{
		assert_t::check_bool(m_graphic_pipeline, "m_graphic_pipeline is not valid");
		return *m_graphic_pipeline;
	}

	void engine_t::init_graphic_pipeline()
	{
		m_graphic_pipeline = std::make_unique<graphic_pipeline_t>(*this);
		assert_t::check_bool(m_graphic_pipeline, "m_graphic_pipeline is not valid");
	}

	void engine_t::shut_graphic_pipeline()
	{
		assert_t::check_bool(m_graphic_pipeline, "m_graphic_pipeline is not valid");
		m_graphic_pipeline = {};
	}

	auto engine_t::get_graphic_executor() const->graphic_executor_t&
	{
		assert_t::check_bool(m_graphic_executor, "m_graphic_executor is not valid");
		return *m_graphic_executor;
	}

	void engine_t::init_graphic_executor()
	{
		m_graphic_executor = std::make_unique<graphic_executor_t>(*this);
		assert_t::check_bool(m_graphic_executor, "m_graphic_executor is not valid");
	}

	void engine_t::shut_graphic_executor()
	{
		assert_t::check_bool(m_graphic_executor, "m_graphic_executor is not valid");
		m_graphic_executor = {};
	}

	auto engine_t::get_graphic_renderer() const->graphic_renderer_t&
	{
		assert_t::check_bool(m_graphic_renderer, "m_graphic_renderer is not valid");
		return *m_graphic_renderer;
	}

	void engine_t::init_graphic_renderer()
	{
		m_graphic_renderer = std::make_unique<graphic_renderer_t>(*this);
		assert_t::check_bool(m_graphic_renderer, "m_graphic_renderer is not valid");
	}

	void engine_t::shut_graphic_renderer()
	{
		assert_t::check_bool(m_graphic_renderer, "m_graphic_renderer is not valid");
		m_graphic_renderer = {};
	}

	auto engine_t::get_scene_system() const->scene_system_t&
	{
		assert_t::check_bool(m_scene_system, "(m_scene_system) is not valid");
		return (*m_scene_system);
	}

	void engine_t::init_scene_system()
	{
		m_scene_system = std::make_unique<scene_system_t>(*this);
		assert_t::check_bool(m_scene_system, "(m_scene_system) is not valid");
	}

	void engine_t::shut_scene_system()
	{
		assert_t::check_bool(m_scene_system, "(m_scene_system) is not valid");
		m_scene_system = {};
	}

	auto engine_t::get_deffered_renderer() const->deffered_renderer_t&
	{
		assert_t::check_bool(m_deffered_renderer, "m_deffered_renderer is not valid");
		return *m_deffered_renderer;
	}

	void engine_t::init_deffered_renderer()
	{
		m_deffered_renderer = std::make_unique<deffered_renderer_t>(*this);
		assert_t::check_bool(m_deffered_renderer, "m_deffered_renderer is not valid");
	}

	void engine_t::shut_deffered_renderer()
	{
		assert_t::check_bool(m_deffered_renderer, "m_deffered_renderer is not valid");
		m_deffered_renderer = {};
	}
}
