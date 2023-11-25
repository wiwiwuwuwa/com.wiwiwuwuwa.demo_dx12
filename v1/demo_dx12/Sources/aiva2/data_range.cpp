#include <pch.h>
#include <aiva2/data_range.hpp>

namespace aiva2
{
    auto data_range_t::get_offset() const -> std::size_t const&
    {
        return m_offset;
    }

    void data_range_t::set_offset(std::size_t const& offset)
    {
        m_offset = offset;
    }

    auto data_range_t::get_size() const -> std::size_t const&
    {
        return m_size;
    }

    void data_range_t::set_size(std::size_t const& size)
    {
        m_size = size;
    }
}
