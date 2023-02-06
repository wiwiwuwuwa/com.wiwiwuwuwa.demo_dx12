#include <pch.h>
#include <aiva2/native/graphic_command_executors.hpp>

#include <aiva2/core/asserts.hpp>
#include <aiva2/core/object.hpp>

namespace aiva2::native
{
	void graphic_command_executors_t::execute_command(core::object_t& command, engine_t& engine)
	{
		get_instance().execute_command_impl(command, engine);
	}

	graphic_command_executors_t& graphic_command_executors_t::get_instance()
	{
		static graphic_command_executors_t instance{};
		return instance;
	}
	
	void graphic_command_executors_t::execute_command_impl(core::object_t& command, engine_t& engine) const
	{
		auto const executor = m_executors.find(typeid(command));
		core::asserts_t::check_true(executor != std::cend(m_executors), "command executor is not found");

		executor->second(command, engine);
	}
}
