#include <pch.h>
#include <aiva2/render_buffer_2d_base.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/texture_2d.hpp>

namespace aiva2
{
	render_buffer_2d_base_t::render_buffer_2d_base_t(engine_t& engine, std::shared_ptr<texture_2d_t> const& resource)
		: impl_type{ engine }
		, m_resource{ resource }
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");
	}

	render_buffer_2d_base_t::~render_buffer_2d_base_t()
	{

	}

	auto render_buffer_2d_base_t::get_resource() const->std::shared_ptr<texture_2d_t> const&
	{
		return m_resource;
	}
}
