#include <pch.h>
#include <aiva2/render_texture_2d_utils.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/render_color_buffer_2d.hpp>
#include <aiva2/render_depth_buffer_2d.hpp>
#include <aiva2/render_texture_2d.hpp>

namespace aiva2
{
	auto render_texture_2d_utils::get_color_cpu_descriptor_handles(render_texture_2d_t const& resource) -> std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		auto handles = std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>{};
		for (auto i = size_t{}; i < resource.num_color_buffers(); i++)
		{
			auto const& buffer = resource.get_color_buffer(i);
			assert_t::check_bool(buffer, "buffer is not valid");
			
			auto const handle = (*buffer).get_cpu_descriptor_handle();
			handles.push_back(handle);
		}
		return handles;
	}

	auto render_texture_2d_utils::get_depth_cpu_descriptor_handle(render_texture_2d_t const& resource) -> std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		auto const& buffer = resource.get_depth_buffer();
		if (!buffer)
		{
			return {};
		}

		return (*buffer).get_cpu_descriptor_handle();
	}
}
