#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	struct render_color_target_t
	{
		// ------------------------------------------------
		
	public:
		virtual auto get_cpu_descriptor_handle() const->D3D12_CPU_DESCRIPTOR_HANDLE = 0;

		virtual void set_state_for_transition() const = 0;
		
		virtual void set_state_for_uav() const = 0;

	protected:
		virtual ~render_color_target_t() = default;
		
		// ------------------------------------------------
	};
}
