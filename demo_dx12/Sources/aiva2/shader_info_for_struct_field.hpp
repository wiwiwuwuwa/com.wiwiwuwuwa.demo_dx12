#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/hlsl_primitive_type.hpp>
#include <aiva2/hlsl_semantic_type.hpp>

namespace aiva2
{
    struct shader_info_for_struct_field_t final : public engine_object_t
    {
        // ------------------------------------------------

    public:
        shader_info_for_struct_field_t(engine_t& engine, std::string const& text);

        ~shader_info_for_struct_field_t() override;

        // ------------------------------------------------

    public:
        auto get_text() const->std::string const&;

    private:
        std::string m_text{};

        // ------------------------------------------------

    public:
        auto get_primitive_type() const->hlsl_primitive_type_t;

        auto get_name() const->std::string const&;

        auto get_semantic_type() const->hlsl_semantic_type_t;

        auto get_semantic_index() const->size_t;

    private:
        void init_self();

        void shut_self();

    private:
        hlsl_primitive_type_t m_primitive_type{};

        std::string m_name{};

        hlsl_semantic_type_t m_semantic_type{};

        size_t m_semantic_index{};

        // ------------------------------------------------
    };
}