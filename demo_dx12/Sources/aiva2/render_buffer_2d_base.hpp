#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
	struct render_buffer_2d_base_t : public implements_t<render_buffer_2d_base_t, engine_object_t>
	{
		// ------------------------------------------------

	public:
		render_buffer_2d_base_t(engine_t& engine, std::shared_ptr<texture_2d_t> const& resource);

		~render_buffer_2d_base_t() override;

		// ------------------------------------------------

	public:
		auto get_resource() const->std::shared_ptr<texture_2d_t> const&;
		
	private:
		std::shared_ptr<texture_2d_t> m_resource{};

		// ------------------------------------------------
	};
}