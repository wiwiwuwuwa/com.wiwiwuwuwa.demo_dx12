#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/core/object.hpp>

namespace aiva2::native
{
	struct gcd_set_viewports_t final : public core::implements_t<gcd_set_viewports_t, core::object_t>
	{
		// ------------------------------------------------
		// set viewports

	public:
		auto get_rect() const->glm::vec4 const&;

		void set_rect(glm::vec4 const& rect);
		
	private:
		glm::vec4 m_rect{};
	};
}
