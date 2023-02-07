#pragma once
#include <aiva2/base.hpp>

#include <aiva2/core/asserts.hpp>
#include <aiva2/native/buffer_format.hpp>

namespace aiva2::native
{
	constexpr auto to_buffer_format(DXGI_FORMAT const format)
	{
		switch (format)
		{
		case DXGI_FORMAT_UNKNOWN:
			return buffer_format_t::UNKNOWN;

		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return buffer_format_t::R8G8B8A8_UNORM;
			
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return buffer_format_t::R32G32B32A32_FLOAT;
			
		case DXGI_FORMAT_D32_FLOAT:
			return buffer_format_t::D32_FLOAT;
			
		default:
			core::asserts_t::check_true(false, "unsupported dxgi format");
			return buffer_format_t::UNKNOWN;
		}
	}

	template <DXGI_FORMAT t_format>
	constexpr auto to_buffer_format()
	{
		return to_buffer_format(t_format);
	}

	// ----------------------------------------------------

	template <DXGI_FORMAT t_format>
	struct to_buffer_format_t : public std::integral_constant<buffer_format_t, to_buffer_format(t_format)> {};

	template <DXGI_FORMAT t_format>
	constexpr auto to_buffer_format_v = to_buffer_format_t<t_format>::value;
}
