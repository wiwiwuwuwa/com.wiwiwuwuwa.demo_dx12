#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct graphic_executor_t final : public implements_t<graphic_executor_t, engine_object_t>
	{
		// ------------------------------------------------
		
	public:
		graphic_executor_t(engine_t& engine);

		~graphic_executor_t() override;
		
	public:
		void execute_command(std::shared_ptr<gpu_cmd_base_t> const& command);

	private:
		void init_graphic_executor();

		void tick_graphic_executor();

		void shut_graphic_executor();
		
	private:
		std::deque<std::shared_ptr<gpu_cmd_base_t>> m_pending_commands{};

		std::deque<std::shared_ptr<gpu_cmd_base_t>> m_executing_commands{};

		// ------------------------------------------------
	};
}
