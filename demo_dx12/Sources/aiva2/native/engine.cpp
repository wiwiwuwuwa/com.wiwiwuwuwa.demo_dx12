#include <pch.h>
#include <aiva2/native/engine.hpp>

#include <aiva2/core/asserts.hpp>
#include <aiva2/core/object_utils.hpp>
#include <aiva2/native/window_system.hpp>

namespace aiva2::native
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

	auto engine_t::get_window_system() const -> window_system_t&
	{
		core::asserts_t::check_true(m_window_system, "window system is not valid");
		return *m_window_system;
	}

	void engine_t::init_window_system()
	{
		m_window_system = core::new_object<window_system_t>(*this);
		core::asserts_t::check_true(m_window_system, "window system is not valid");
	}

	void engine_t::shut_window_system()
	{
		core::asserts_t::check_true(m_window_system, "window system is not valid");
		m_window_system = {};
	}
}
