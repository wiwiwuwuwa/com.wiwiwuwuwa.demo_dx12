#include <pch.h>
#include <aiva2/shader_info_for_struct_field.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/enum_utils.hpp>
#include <aiva2/hlsl_primitive_type_utils.hpp>
#include <aiva2/hlsl_semantic_type_utils.hpp>

namespace aiva2
{
    shader_info_for_struct_field_t::shader_info_for_struct_field_t(engine_t& engine, std::string const& text)
        : impl_type{ engine }
        , m_text{ text }
    {
        init_self();
    }

    shader_info_for_struct_field_t::~shader_info_for_struct_field_t()
    {
        shut_self();
    }

    auto shader_info_for_struct_field_t::get_text() const->std::string const&
    {
        return m_text;
    }

    auto shader_info_for_struct_field_t::get_primitive_type() const->hlsl_primitive_type_t
    {
        return m_primitive_type;
    }

    auto shader_info_for_struct_field_t::get_name() const->std::string const&
    {
        return m_name;
    }

    auto shader_info_for_struct_field_t::get_semantic_type() const->hlsl_semantic_type_t
    {
        return m_semantic_type;
    }

    auto shader_info_for_struct_field_t::get_semantic_index() const->size_t
    {
        return m_semantic_index;
    }

    void shader_info_for_struct_field_t::init_self()
    {
        auto const regex = std::regex{ R"(\b(\w+)\s+(\w+)\s*(?::\s*([a-z_A-Z]+)(\d+)?)?)", std::regex::icase | std::regex::optimize };
        auto match = std::smatch{};

        assert_t::check_bool(std::regex_search(m_text, match, regex), "failed to find struct field info");
        assert_t::check_bool(match.ready(), "failed to find struct field info");
        assert_t::check_bool(std::size(match) >= 3 && std::size(match) <= 5, "failed to find struct field info");
        
        if (std::size(match) >= 2)
        {
            auto const& primitive_type_match = match[1];
            assert_t::check_bool(primitive_type_match.matched, "(primitive_type_match) is not valid");

            auto const& primitive_type_str = primitive_type_match.str();
            assert_t::check_bool(!std::empty(primitive_type_str), "(primitive_type_str) is not valid");
            
            auto const primitive_type_enum = enum_from_string<hlsl_primitive_type_t>(primitive_type_str);
            assert_t::check_bool(is_valid(primitive_type_enum), "(primitive_type_enum) is not valid");

            m_primitive_type = primitive_type_enum;
        }

        if (std::size(match) >= 3)
        {
            auto const& name_match = match[2];
            assert_t::check_bool(name_match.matched, "(name_match) is not valid");

            auto const& name_str = name_match.str();
            assert_t::check_bool(!std::empty(name_str), "(name_str) is not valid");

            m_name = name_str;
        }

        if (std::size(match) >= 4)
        {
            auto const& semantic_type_match = match[3];
            assert_t::check_bool(semantic_type_match.matched, "(semantic_type_match) is not valid");

            auto const& semantic_type_str = semantic_type_match.str();
            assert_t::check_bool(!std::empty(semantic_type_str), "(semantic_type_str) is not valid");
            
			auto const semantic_type_enum = enum_from_string<hlsl_semantic_type_t>(semantic_type_str);
            assert_t::check_bool(is_valid(semantic_type_enum), "(semantic_type_enum) is not valid");

            m_semantic_type = semantic_type_enum;
        }

        if (std::size(match) >= 5)
        {
            auto const& semantic_index_match = match[4];
            assert_t::check_bool(semantic_index_match.matched, "(semantic_index_match) is not valid");

            auto const& semantic_index_str = semantic_index_match.str();
            assert_t::check_bool(!std::empty(semantic_index_str), "(semantic_index_str) is not valid");

            auto const semantic_index = static_cast<size_t>(std::stoull(semantic_index_str));
            assert_t::check_bool(semantic_index >= decltype(semantic_index){}, "(semantic_index) is not valid");

            m_semantic_index = semantic_index;
        }
    }

    void shader_info_for_struct_field_t::shut_self()
    {
        m_semantic_index = {};
        m_semantic_type = {};
        m_name = {};
        m_primitive_type = {};
    }
}
