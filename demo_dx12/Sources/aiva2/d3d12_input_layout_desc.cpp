#include <pch.h>
#include <aiva2/d3d12_input_layout_desc.hpp>

#include <aiva2/assert.hpp>\
#include <aiva2/shader_info.hpp>

namespace aiva2
{
    d3d12_input_layout_desc_t::d3d12_input_layout_desc_t(shader_info_t const& shader_info)
    {
        init_data(shader_info);
    }

    d3d12_input_layout_desc_t::~d3d12_input_layout_desc_t()
    {
        shut_data();
    }

    auto d3d12_input_layout_desc_t::get_data() const->D3D12_INPUT_LAYOUT_DESC const&
    {
        return m_data;
    }

    void d3d12_input_layout_desc_t::init_data(shader_info_t const&)
    {
        assert_t::check_bool(false, "TODO: IMPLEMENT ME");
    }

    void d3d12_input_layout_desc_t::shut_data()
    {
        m_data = {};
    }
}
