#include <pch.h>
#include <aiva2/material_property_block.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buf.hpp>
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

    auto material_property_block_t::get_value_buffer_ptr() const -> std::shared_ptr<cbv_buf_t const> const&
    {
        return m_values_buffer;
    }

    void material_property_block_t::get_byte_value(std::string_view const& field_name, boost::span<std::byte> const& out_value) const
    {
        get_shader_info_ref().get_byte_value(field_name, get_value_buffer_ref().get_resource_ref(), out_value);
    }

    void material_property_block_t::set_byte_value(std::string_view const& field_name, boost::span<std::byte const> const& in_value) const
    {
        get_shader_info_ref().set_byte_value(field_name, in_value, get_value_buffer_ref().get_resource_ref());
    }

    auto material_property_block_t::get_value_buffer_ref() const -> cbv_buf_t const&
    {
        assert_t::check_bool(m_values_buffer, "(m_values_buffer) is not valid");
        return (*m_values_buffer);
    }

    void material_property_block_t::init_values_buffer()
    {
        auto res_inf = buf_info_t{};
        res_inf.set_memory(buffer_memory_t::CPU_TO_GPU);
        res_inf.set_size(get_shader_info_ref().get_byte_size());
        res_inf.set_support_shader_resource(true);

        auto res_obj = std::make_shared<buf_t>(get_engine(), res_inf);
        assert_t::check_bool(res_obj, "(res_obj) is not valid");

        auto eye_inf = cbv_buf_info_t{};
        eye_inf.set_size(get_shader_info_ref().get_byte_size());

        auto eye_obj = std::make_shared<cbv_buf_t>(get_engine(), res_obj, eye_inf);
        assert_t::check_bool(eye_obj, "(eye_obj) is not valid");

        m_values_buffer = eye_obj;
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
