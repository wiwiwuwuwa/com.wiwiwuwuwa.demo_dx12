#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/implements.hpp>
#include <aiva2/core/object.hpp>

namespace aiva2::native
{
	struct engine_t final : public core::implements_t<engine_t, core::object_t>
	{
		// ------------------------------------------------

	public:
		engine_t();

		~engine_t() override;

		// ------------------------------------------------
	};
}
