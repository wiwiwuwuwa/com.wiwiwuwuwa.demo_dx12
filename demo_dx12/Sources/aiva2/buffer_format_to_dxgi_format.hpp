#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_format.hpp>

namespace aiva2
{
	constexpr auto to_dxgi_format(buffer_format_t const format)
	{
		switch (format)
		{
		case buffer_format_t::UNKNOWN:
			return DXGI_FORMAT_UNKNOWN;

		case buffer_format_t::R8G8B8A8_UNORM:
			return DXGI_FORMAT_R8G8B8A8_UNORM;

		case buffer_format_t::R32G32B32A32_FLOAT:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case buffer_format_t::D32_FLOAT:
			return DXGI_FORMAT_D32_FLOAT;

		default:
			assert_t::check_bool(false, "unsupported buffer format");
			return DXGI_FORMAT_UNKNOWN;
		}
	}
	
	template <buffer_format_t t_format>
	constexpr auto to_dxgi_format()
	{
		return to_dxgi_format(t_format);
	}

	// ----------------------------------------------------
	
	template <buffer_format_t t_format>
	struct to_dxgi_format_t : public std::integral_constant<DXGI_FORMAT, to_dxgi_format(t_format)> {};

	template <buffer_format_t t_format>
	constexpr auto to_dxgi_format_v = to_dxgi_format_t<t_format>::value;
}
