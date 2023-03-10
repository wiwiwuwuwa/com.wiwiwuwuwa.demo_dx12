#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/render_texture_base.hpp>

namespace aiva2
{
	struct render_texture_2d_base_t : public implements_t<render_texture_2d_base_t, render_texture_base_t>
	{
		// ------------------------------------------------

	protected:
		render_texture_2d_base_t(engine_t& engine);

		~render_texture_2d_base_t() override;

		// ------------------------------------------------

	public:
		auto get_resource() const->std::shared_ptr<texture_2d_t> const&;
		
	protected:
		std::shared_ptr<texture_2d_t> m_resource{};

		// ------------------------------------------------
	};
}