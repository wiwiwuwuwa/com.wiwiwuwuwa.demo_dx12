#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/native/engine_object.hpp>

namespace aiva2::native
{
	struct graphic_executor_t final : public core::implements_t<graphic_executor_t, engine_object_t>
	{
		// ------------------------------------------------
		// graphic executor

	public:
		graphic_executor_t(engine_t& engine);

		~graphic_executor_t() override;

	public:
		void execute_command(std::shared_ptr<core::object_t> const& command);

	private:
		void init_graphic_executor();

		void tick_graphic_executor();

		void shut_graphic_executor();

	private:
		std::deque<std::shared_ptr<core::object_t>> m_pending_commands{};

		std::deque<std::shared_ptr<core::object_t>> m_executing_commands{};
	};
}
