#include <pch.h>
#include <aiva2/native/graphic_executor.hpp>

#include <aiva2/core/asserts.hpp>
#include <aiva2/native/engine.hpp>
#include <aiva2/native/graphic_command_executors.hpp>
#include <aiva2/native/graphic_pipeline.hpp>

namespace aiva2::native
{
	graphic_executor_t::graphic_executor_t(engine_t& engine) : impl_type{ engine }
	{
		init_graphic_executor();
	}

	graphic_executor_t::~graphic_executor_t()
	{
		shut_graphic_executor();
	}

	void graphic_executor_t::execute_command(std::shared_ptr<core::object_t> const& command)
	{
		core::asserts_t::check_true(command, "command is not valid");
		m_pending_commands.push_back(command);
	}

	void graphic_executor_t::init_graphic_executor()
	{
		m_pending_commands = {};
		m_executing_commands = {};
		
		get_engine().get_graphic_pipeline().get_on_populate_commands().attach_listener(&this_type::tick_graphic_executor, this);
	}

	void graphic_executor_t::tick_graphic_executor()
	{
		while (!std::empty(m_executing_commands))
		{
			m_executing_commands.pop_front();
		}

		while (!std::empty(m_pending_commands))
		{
			auto const& command = m_pending_commands.front();
			core::asserts_t::check_true(command, "command is not valid");
			
			graphic_command_executors_t::execute_command(*command, get_engine());

			m_executing_commands.push_back(command);
			m_pending_commands.pop_front();
		}
	}

	void graphic_executor_t::shut_graphic_executor()
	{
		get_engine().get_graphic_pipeline().get_on_populate_commands().remove_listener(&this_type::tick_graphic_executor, this);

		m_executing_commands = {};
		m_pending_commands = {};
	}
}
