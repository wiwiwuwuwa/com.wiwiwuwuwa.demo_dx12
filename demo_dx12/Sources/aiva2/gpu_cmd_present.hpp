#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_cmd_base.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct gpu_cmd_present_t final : public implements_t<gpu_cmd_present_t, gpu_cmd_base_t>
	{
		// ------------------------------------------------

	public:
		gpu_cmd_present_t(engine_t& engine);

		~gpu_cmd_present_t() override;

	public:
		void execute() override;

	private:
		void execute_resource_barrier();

		// ------------------------------------------------
	};
}
