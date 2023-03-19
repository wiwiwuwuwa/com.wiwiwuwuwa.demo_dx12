#include <pch.h>
#include <aiva2/gpu_cmd_set_render_target.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/render_target.hpp>

namespace aiva2
{
	gpu_cmd_set_render_target_t::gpu_cmd_set_render_target_t(engine_t& engine)
		: impl_type{ engine }
	{
		
	}

	gpu_cmd_set_render_target_t::~gpu_cmd_set_render_target_t()
	{
		
	}

	void gpu_cmd_set_render_target_t::execute()
	{
		assert_t::check_bool(m_render_target, "m_render_target is not valid");
		(*m_render_target).init_for_rendering();
	}

	auto gpu_cmd_set_render_target_t::get_render_target() const->std::shared_ptr<render_target_t> const&
	{
		return m_render_target;
	}

	void gpu_cmd_set_render_target_t::set_render_target(std::shared_ptr<render_target_t> const& render_target)
	{
		m_render_target = render_target;
	}
}
