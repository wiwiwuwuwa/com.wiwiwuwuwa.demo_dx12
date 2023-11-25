#include <pch.h>
#include <aiva2/vbv_buf_info.hpp>

namespace aiva2
{
    auto vbv_buf_info_t::get_stride() const->size_t
    {
        return m_stride;
    }

    void vbv_buf_info_t::set_stride(size_t const stride)
    {
        m_stride = stride;
    }
}
