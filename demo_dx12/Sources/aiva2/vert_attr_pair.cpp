#include <pch.h>
#include <aiva2/vert_attr_pair.hpp>

namespace aiva2
{
    auto vert_attr_pair_t::get_type() const->vert_attr_type_t
    {
        return m_type;
    }

    void vert_attr_pair_t::set_type(vert_attr_type_t const type)
    {
        m_type = type;
    }

    auto vert_attr_pair_t::get_indx() const->std::size_t
    {
        return m_indx;
    }

    void vert_attr_pair_t::set_indx(std::size_t const indx)
    {
        m_indx = indx;
    }
}
