#pragma once
#include <aiva2/base.hpp>

namespace aiva2::native
{
	struct gce_set_viewports_t final
	{
		// ------------------------------------------------
		// set viewports

	private:
		gce_set_viewports_t() = delete;

	public:
		static void execute(gcd_set_viewports_t& command, engine_t& engine);

	private:
		static void execute_set_scissor_rects(gcd_set_viewports_t& command, engine_t& engine);
		
		static void execute_set_viewports(gcd_set_viewports_t& command, engine_t& engine);
	};
}
