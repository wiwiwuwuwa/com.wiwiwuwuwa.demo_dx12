#pragma once

#include <d3d12.h>
#include <aiva/assert.hpp>
#include <aiva/graphic_format.hpp>

namespace aiva
{
    // ----------------------------------------------------

	constexpr auto to_dxgi_format(graphic_format_t const format)
	{
		switch (format)
		{
		case graphic_format_t::UNKNOWN:
			return DXGI_FORMAT_UNKNOWN;

		case graphic_format_t::R8G8B8A8_UNORM:
			return DXGI_FORMAT_R8G8B8A8_UNORM;

		default:
			assert_t::check_bool(false, "(format) is not valid");
			return DXGI_FORMAT_UNKNOWN;
		}
	}

	// ----------------------------------------------------
}
