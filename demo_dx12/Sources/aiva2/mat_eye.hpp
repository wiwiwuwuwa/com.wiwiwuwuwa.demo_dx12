#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_eye.hpp>

namespace aiva2
{
	struct mat_eye_t : public implements_t<mat_eye_t, gpu_eye_t>
	{
		// ------------------------------------------------

	protected:
		mat_eye_t(engine_t& engine);

		~mat_eye_t() override;

		// ------------------------------------------------
	};
}
