#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/implements.hpp>

namespace aiva2
{
    struct material_base_t : public implements_t<material_base_t, engine_object_t>
    {
        // ------------------------------------------------

    protected:
        material_base_t(engine_t& engine);

        ~material_base_t() override;

        // ------------------------------------------------

    public:
        auto get_shader_ref() const->shader_base_t const&;

        auto get_shader_ptr() const->std::shared_ptr<shader_base_t const>;

    protected:
        virtual auto get_shader_imp() const->std::shared_ptr<shader_base_t const> = 0;

        // ------------------------------------------------
    };
}
