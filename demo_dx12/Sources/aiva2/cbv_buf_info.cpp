#include <pch.h>
#include <aiva2/cbv_buf_info.hpp>

namespace aiva2
{
    cbv_buf_info_t::cbv_buf_info_t()
    {

    }

    cbv_buf_info_t::~cbv_buf_info_t()
    {

    }

    auto cbv_buf_info_t::get_size() const->size_t
    {
        return m_size;
    }

    void cbv_buf_info_t::set_size(size_t const size)
    {
        m_size = size;
    }
}
