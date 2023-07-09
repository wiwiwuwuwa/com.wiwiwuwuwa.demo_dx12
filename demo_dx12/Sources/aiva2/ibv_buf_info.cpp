#include <pch.h>
#include <aiva2/ibv_buf_info.hpp>

namespace aiva2
{
    auto ibv_buf_info_t::get_format() const->buffer_format_t
    {
        return m_format;
    }

    void ibv_buf_info_t::set_format(buffer_format_t const format)
    {
        m_format = format;
    }
}
