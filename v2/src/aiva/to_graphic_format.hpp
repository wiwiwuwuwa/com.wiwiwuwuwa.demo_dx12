#pragma once

#include <d3d12.h>
#include <aiva/assert.hpp>
#include <aiva/graphic_format.hpp>

namespace aiva
{
	// ----------------------------------------------------

	constexpr auto to_graphic_format(DXGI_FORMAT const format)
	{
		switch (format)
		{
		case DXGI_FORMAT_UNKNOWN:
			return graphic_format_t::UNKNOWN;

		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return graphic_format_t::R8G8B8A8_UNORM;

		default:
			assert_t::check_bool(false, "(format) is not valid");
			return graphic_format_t::UNKNOWN;
		}
	}

	// ----------------------------------------------------
}
