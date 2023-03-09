#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	struct render_texture_2d_utils final
	{
		// ------------------------------------------------

	private:
		render_texture_2d_utils() = delete;
		
	public:
		static auto get_color_cpu_descriptor_handles(render_texture_2d_t const& resource) -> std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>;
		
		static auto get_depth_cpu_descriptor_handle(render_texture_2d_t const& resource) -> std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>;

		// ------------------------------------------------
	};
}
