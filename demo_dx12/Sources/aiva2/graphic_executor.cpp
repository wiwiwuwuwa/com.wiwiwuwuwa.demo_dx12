#include <pch.h>
#include <aiva2/graphic_executor.hpp>

#include <aiva2/asserts.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_cmd_base.hpp>
#include <aiva2/graphic_pipeline.hpp>

namespace aiva2
{
	graphic_executor_t::graphic_executor_t(engine_t& engine)
		: impl_type{ engine }
	{
		init_graphic_executor();
	}

	graphic_executor_t::~graphic_executor_t()
	{
		shut_graphic_executor();
	}
	
	void graphic_executor_t::execute_command(std::shared_ptr<gpu_cmd_base_t> const& command)
	{
		asserts_t::check_true(command, "command is not valid");
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
			asserts_t::check_true(command, "command is not valid");

			(*command).execute();

			m_executing_commands.push_back(command);
			m_pending_commands.pop_front();
		}
	}

	void graphic_executor_t::shut_graphic_executor()
	{
		get_engine().get_graphic_pipeline().get_on_populate_commands().detach_listener(&this_type::tick_graphic_executor, this);

		m_executing_commands = {};
		m_pending_commands = {};
	}
}
