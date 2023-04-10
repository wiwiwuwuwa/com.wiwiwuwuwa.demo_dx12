#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>

namespace aiva2
{
    struct shader_info_for_func_t final : public implements_t<shader_info_for_func_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        shader_info_for_func_t(engine_t &engine, std::string_view const& text);

        ~shader_info_for_func_t() override;

        // ------------------------------------------------

    public:
        auto get_text() const -> std::string const&;

    private:
        std::string m_text{};

        // ------------------------------------------------

    private:
        void init_self();

        void shut_self();

        // ------------------------------------------------

    public:
        auto get_name() const -> std::string const&;

    private:
        std::string m_name{};

        // ------------------------------------------------

    public:
        auto get_input_type(size_t const index) const -> std::string const&;

        auto num_input_type() const -> size_t;

    private:
        std::vector<std::string> m_input_types{};

        // ------------------------------------------------

    public:
        auto get_output_type() const -> std::string const&;

    private:
        std::string m_output_type{};

        // ------------------------------------------------
    };
}
