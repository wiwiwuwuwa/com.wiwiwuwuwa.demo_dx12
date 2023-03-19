#include <pch.h>
#include <aiva2/uav_res_info.hpp>

namespace aiva2
{
	auto uav_res_info_t::get_format() const->buffer_format_t
	{
		return m_format;
	}

	void uav_res_info_t::set_format(buffer_format_t const format)
	{
		m_format = format;
	}
}
