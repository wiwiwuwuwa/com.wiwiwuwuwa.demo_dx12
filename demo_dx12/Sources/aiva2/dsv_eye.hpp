#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_eye.hpp>

namespace aiva2
{
	struct dsv_eye_t : public implements_t<dsv_eye_t, gpu_eye_t>
	{
		// ------------------------------------------------

	protected:
		dsv_eye_t(engine_t& engine);

		~dsv_eye_t() override;

		// ------------------------------------------------
	};
}
