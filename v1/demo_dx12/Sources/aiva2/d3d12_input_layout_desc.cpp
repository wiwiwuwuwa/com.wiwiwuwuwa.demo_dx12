#include <pch.h>
#include <aiva2/d3d12_input_layout_desc.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_format_utils.hpp>
#include <aiva2/hlsl_primitive_type_utils.hpp>
#include <aiva2/hlsl_semantic_type_utils.hpp>
#include <aiva2/shader_info.hpp>
#include <aiva2/shader_info_for_code.hpp>
#include <aiva2/shader_info_for_func.hpp>
#include <aiva2/shader_info_for_meta.hpp>
#include <aiva2/shader_info_for_struct.hpp>
#include <aiva2/shader_info_for_struct_field.hpp>

namespace aiva2
{
    d3d12_input_layout_desc_t::d3d12_input_layout_desc_t(shader_info_t const& shader_info)
    {
        init_element_descs(shader_info);
        init_layout_desc(shader_info);
    }

    d3d12_input_layout_desc_t::~d3d12_input_layout_desc_t()
    {
        shut_layout_desc();
        shut_element_descs();
    }

    auto d3d12_input_layout_desc_t::get_data() const->D3D12_INPUT_LAYOUT_DESC const&
    {
        return m_layout_desc;
    }

    void d3d12_input_layout_desc_t::init_element_descs(shader_info_t const& shader_info)
    {
        auto const& entry_for_vert_name = shader_info.get_meta_block().get_entry_for_vert();
        assert_t::check_bool(!std::empty(entry_for_vert_name), "(entry_for_vert_name) is not valid");

        auto const& entry_for_vert_func = shader_info.get_code_block().get_func_ref(entry_for_vert_name);
        assert_t::check_bool(entry_for_vert_func.num_input_type() == 1, "(entry_for_vert_func) is not valid");

        auto const& input_layout_struct_name = entry_for_vert_func.get_input_type(0);
        assert_t::check_bool(!std::empty(input_layout_struct_name), "(input_layout_struct_name) is not valid");
        auto const& input_layout_struct_data = shader_info.get_code_block().get_struct_ref(input_layout_struct_name);

        m_element_descs = {};

        for (auto i = size_t{}; i < input_layout_struct_data.num_input_layout_slot(); i++)
        {
            auto const& input_layout_field_data = input_layout_struct_data.get_input_layout_field_ref(i);

            auto& element_desc = m_element_descs.emplace_back();
            
            {
                auto const semantic_type = input_layout_field_data.get_semantic_type();
                assert_t::check_bool(is_valid(semantic_type), "(semantic_type) is not valid");

                auto const& semantic_name = to_string(semantic_type);
                assert_t::check_bool(semantic_name, "(semantic_name) is not valid");

                element_desc.SemanticName = semantic_name;
            }

            {
                auto const semantic_index = static_cast<UINT>(input_layout_field_data.get_semantic_index());
                element_desc.SemanticIndex = semantic_index;
            }

            {
                auto const primitive_type = input_layout_field_data.get_primitive_type();
                assert_t::check_bool(is_valid(primitive_type), "(primitive_type) is not valid");

                auto buffer_format = buffer_format_t{}; from_hlsl_primitive_type(primitive_type, buffer_format);
                assert_t::check_bool(is_valid(buffer_format), "(buffer_format) is not valid");

                auto const format = to_dxgi_format(buffer_format);
                assert_t::check_bool(format != DXGI_FORMAT_UNKNOWN, "(format) is not valid");

                element_desc.Format = format;
            }

            {
                auto const input_slot = static_cast<UINT>(std::size(m_element_descs) - size_t{ 1 });
                element_desc.InputSlot = input_slot;
            }

            {
                element_desc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
                element_desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
                element_desc.InstanceDataStepRate = {};
            }
        }
    }

    void d3d12_input_layout_desc_t::shut_element_descs()
    {
        m_element_descs = {};
    }

    void d3d12_input_layout_desc_t::init_layout_desc(shader_info_t const&)
    {
        auto layout_desc = D3D12_INPUT_LAYOUT_DESC{};
        layout_desc.pInputElementDescs = std::data(m_element_descs);
        layout_desc.NumElements = static_cast<UINT>(std::size(m_element_descs));

        m_layout_desc = layout_desc;
    }

    void d3d12_input_layout_desc_t::shut_layout_desc()
    {
        m_layout_desc = {};
    }
}
