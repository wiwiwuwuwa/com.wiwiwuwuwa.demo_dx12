#include <pch.h>
#include <aiva2/mat_eye.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_eye_init_info.hpp>
#include <aiva2/graphic_hardware.hpp>

namespace aiva2
{
	mat_eye_t::mat_eye_t(engine_t& engine)
		: impl_type
		{
			/*engine*/ engine,
			/*init_info*/ impl_type::init_info_type{}
				.set_descriptor_heap_type(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
				.set_descriptor_heap_flag(D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
		}
	{
		
	}

	mat_eye_t::~mat_eye_t()
	{
		
	}
}
