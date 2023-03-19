#include <pch.h>
#include <aiva2/rtv_eye.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_eye_init_info.hpp>
#include <aiva2/rtv_eye_init_info.hpp>

namespace aiva2
{
	rtv_eye_t::rtv_eye_t(engine_t& engine, init_info_type const& init_info)
		: impl_type
		{
			/*engine*/ engine,
			/*init_info*/ impl_type::init_info_type{}
				.set_descriptor_heap_type(init_info.get_descriptor_heap_type())
				.set_descriptor_heap_flag(D3D12_DESCRIPTOR_HEAP_FLAG_NONE)
		}
	{

	}

	rtv_eye_t::~rtv_eye_t()
	{

	}
}
