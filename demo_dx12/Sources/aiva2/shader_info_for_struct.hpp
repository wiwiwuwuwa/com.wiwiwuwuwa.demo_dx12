#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>

namespace aiva2
{
    struct shader_info_for_struct_t final : public implements_t<shader_info_for_struct_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        shader_info_for_struct_t(engine_t &engine, std::string_view const& text);

        ~shader_info_for_struct_t() override;

        // ------------------------------------------------

    public:
        auto get_text() const -> std::string const&;

    private:
        std::string m_text{};

        // ------------------------------------------------

    public:
        auto get_name() const -> std::string const&;

    private:
        void init_name();

        void shut_name();

    private:
        std::string m_name{};

        // ------------------------------------------------

    public:
        auto get_field_ref(size_t const index) const -> shader_info_for_struct_field_t const&;

        auto get_field_ref(std::string_view const& name) const -> shader_info_for_struct_field_t const&;

        auto get_field_ptr(size_t const index) const -> std::shared_ptr<shader_info_for_struct_field_t const>;

        auto get_field_ptr(std::string_view const& name) const -> std::shared_ptr<shader_info_for_struct_field_t const>;

        auto num_field() const -> size_t;

    private:
        void init_fields();

        void shut_fields();

    private:
        std::vector<std::shared_ptr<shader_info_for_struct_field_t>> m_fields{};

        // ------------------------------------------------
    };
}
