#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/material_base.hpp>

namespace aiva2
{
    struct compute_material_t final : public implements_t<compute_material_t, material_base_t>
    {
        // ------------------------------------------------

    public:
        compute_material_t(engine_t& engine, nlohmann::json const& json);

        ~compute_material_t() override;

        // ------------------------------------------------

    public:
        auto get_shader_ref() const->compute_shader_t const&;

        auto get_shader_ptr() const->std::shared_ptr<compute_shader_t const>;

    protected:
        auto get_shader_imp() const->std::shared_ptr<shader_base_t const> override;

    private:
        void init_shader(nlohmann::json const& json);

        void shut_shader();

    private:
        std::shared_ptr<compute_shader_t const> m_shader{};

        // ------------------------------------------------
    };
}
