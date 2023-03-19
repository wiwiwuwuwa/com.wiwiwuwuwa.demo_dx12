#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_eye.hpp>

namespace aiva2
{
	struct rtv_eye_t : public implements_t<rtv_eye_t, gpu_eye_t>
	{
		// ------------------------------------------------

	protected:
		rtv_eye_t(engine_t& engine);

		~rtv_eye_t() override;
		
		// ------------------------------------------------
	};
}
