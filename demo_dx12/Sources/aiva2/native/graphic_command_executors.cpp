#include <pch.h>
#include <aiva2/native/graphic_command_executors.hpp>

#include <aiva2/core/asserts.hpp>
#include <aiva2/core/object.hpp>

namespace aiva2::native
{
	std::unordered_map<std::type_index, graphic_command_executors_t::executor_type> graphic_command_executors_t::m_executors{};

	void graphic_command_executors_t::execute_command(core::object_t& command, engine_t& engine)
	{
		auto const executor = m_executors.find(typeid(command));
		core::asserts_t::check_true(executor != std::cend(m_executors), "command executor is not found");

		executor->second(command, engine);
	}
}
