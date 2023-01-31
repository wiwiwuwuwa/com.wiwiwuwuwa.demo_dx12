#pragma once
#include <pch.h>

#include <aiva2/core/implements.h>
#include <aiva2/core/object.h>
#include <aiva2/native/engine_fwd.h>

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
