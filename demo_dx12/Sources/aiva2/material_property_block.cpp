#include <pch.h>
#include <aiva2/material_property_block.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/cbv_buf.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/shader_info_for_struct.hpp>

namespace aiva2
{
    material_property_block_t::material_property_block_t(std::shared_ptr<shader_info_for_struct_t> const& shader_info)
        : impl_type{ (*shader_info).get_engine() }
        , m_shader_info{ shader_info }
    {
        init_values_buffer();
    }

    material_property_block_t::~material_property_block_t()
    {
        shut_values_buffer();
    }

    auto material_property_block_t::get_shader_info_ptr() const -> std::shared_ptr<shader_info_for_struct_t const> const&
    {
        return m_shader_info;
    }

    auto material_property_block_t::get_shader_info_ref() const -> shader_info_for_struct_t const&
    {
        assert_t::check_bool(m_shader_info, "(m_shader_info) is not valid");
        return (*m_shader_info);
    }

    auto material_property_block_t::get_value(std::string_view const& name) const -> std::any
    {
        assert_t::check_bool(false, "TODO: IMPLEMENT ME");
        return {};
    }

    auto material_property_block_t::get_value(size_t const index) const -> std::any
    {
        assert_t::check_bool(false, "TODO: IMPLEMENT ME");
        return {};
    }

    auto material_property_block_t::num_value() const -> size_t
    {
        assert_t::check_bool(false, "TODO: IMPLEMENT ME");
        return {};
    }

    auto material_property_block_t::set_value(std::string_view const& name, std::any const& value) -> void
    {
        assert_t::check_bool(false, "TODO: IMPLEMENT ME");
    }

    auto material_property_block_t::set_value(size_t const index, std::any const& value) -> void
    {
        assert_t::check_bool(false, "TODO: IMPLEMENT ME");
    }

    auto material_property_block_t::get_value_buffer_ptr() const -> std::shared_ptr<cbv_buf_t const> const&
    {
        return m_values_buffer;
    }

    auto material_property_block_t::get_value_buffer_ref() const -> cbv_buf_t const&
    {
        assert_t::check_bool(m_values_buffer, "(m_values_buffer) is not valid");
        return (*m_values_buffer);
    }

    void material_property_block_t::init_values_buffer()
    {
        assert_t::check_bool(false, "TODO: IMPLEMENT ME");
    }

    void material_property_block_t::shut_values_buffer()
    {
        assert_t::check_bool(m_values_buffer, "(m_values_buffer) is not valid");
        m_values_buffer = {};
    }

    auto material_property_block_t::init_for_rendering() const -> std::vector<D3D12_RESOURCE_BARRIER>
    {
        return get_value_buffer_ref().init_for_rendering();
    }

    auto material_property_block_t::get_gpu_virtual_address() const -> D3D12_GPU_VIRTUAL_ADDRESS
    {
        return get_value_buffer_ref().get_gpu_virtual_address();
    }

    auto material_property_block_t::shut_for_rendering() const -> std::vector<D3D12_RESOURCE_BARRIER>
    {
        return get_value_buffer_ref().shut_for_rendering();
    }
}
