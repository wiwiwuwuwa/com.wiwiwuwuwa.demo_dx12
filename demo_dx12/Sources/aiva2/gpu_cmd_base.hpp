#pragma once
#include <aiva2/_core.hpp>
#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct gpu_cmd_base_t : public implements_t<gpu_cmd_base_t, engine_object_t>
	{
		// ------------------------------------------------

	protected:
		gpu_cmd_base_t(engine_t& engine);

		~gpu_cmd_base_t() override;

	public:
		virtual void execute() = 0;

		// ------------------------------------------------
	};
}
