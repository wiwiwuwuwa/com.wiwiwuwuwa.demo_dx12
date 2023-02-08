#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/core/object.hpp>

namespace aiva2::native
{
	struct engine_object_t : public core::implements_t<engine_object_t, core::object_t>
	{
		// ------------------------------------------------
		// engine object

	protected:
		engine_object_t(engine_t& engine);

		~engine_object_t() override;

	public:
		auto get_engine() const->engine_t&;

	private:
		engine_t& m_engine;
	};
}
