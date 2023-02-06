#include <pch.h>
#include <aiva2/native/gcd_set_viewports.hpp>

namespace aiva2::native
{
	auto gcd_set_viewports_t::get_rect() const->glm::vec4 const&
	{
		return m_rect;
	}

	void gcd_set_viewports_t::set_rect(glm::vec4 const& rect)
	{
		m_rect = rect;
	}
}