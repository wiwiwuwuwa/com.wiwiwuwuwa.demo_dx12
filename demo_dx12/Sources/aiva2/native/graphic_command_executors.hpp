#pragma once
#include <aiva2/base.hpp>

namespace aiva2::native
{
	struct graphic_command_executors_t final
	{
		// ------------------------------------------------
		// graphic command executors

	private:
		using executor_type = std::function<void(core::object_t&, engine_t&)>;

	private:
		graphic_command_executors_t() = delete;

	public:
		template <typename t_command_data, typename t_command_exec>
		static void register_executor();

		static void execute_command(core::object_t& command, engine_t& engine);
		
	private:
		static std::unordered_map<std::type_index, executor_type> m_executors;
	};
}

// --------------------------------------------------------

#include <aiva2/core/asserts.hpp>

namespace aiva2::native
{
	template <typename t_command_data, typename t_command_exec>
	void graphic_command_executors_t::register_executor()
	{
		m_executors.insert_or_assign
		(
			typeid(t_command_data),
			[](core::object_t& basic_command, engine_t& engine)
			{
				auto const specific_command = dynamic_cast<t_command_data*>(&basic_command);
				core::asserts_t::check_true(specific_command, "specific command is not valid");

				t_command_exec::execute(*specific_command, engine);
			}
		);
	}
}
