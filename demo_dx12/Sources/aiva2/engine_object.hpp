#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/object.hpp>

namespace aiva2
{
	struct engine_object_t : public implements_t<engine_object_t, object_t>
	{
		// ------------------------------------------------

	protected:
		engine_object_t(engine_t& engine);

		~engine_object_t() override;

		// ------------------------------------------------

	public:
		auto get_engine() const->engine_t&;

	private:
		engine_t& m_engine;

		// ------------------------------------------------
	};
}
