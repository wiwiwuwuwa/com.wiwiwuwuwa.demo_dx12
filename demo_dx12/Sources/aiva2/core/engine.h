#pragma once
#include <pch.h>

#include <aiva2/core/engine_fwd.h>
#include <aiva2/core/implements.h>
#include <aiva2/core/object.h>

namespace aiva2::core
{
	struct engine_t final : public implements_t<engine_t, object_t>
	{
		// ------------------------------------------------

	public:
		engine_t();

		~engine_t() override;

		// ------------------------------------------------
	};
}
