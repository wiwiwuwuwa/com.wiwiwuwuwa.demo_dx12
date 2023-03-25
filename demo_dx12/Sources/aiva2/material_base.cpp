#include <pch.h>
#include <aiva2/material_base.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>

namespace aiva2
{
    material_base_t::material_base_t(engine_t& engine)
        : impl_type{ engine }
    {
        
    }

    material_base_t::~material_base_t()
    {

    }
    
    auto material_base_t::get_shader_ref() const->shader_base_t const&
    {
        auto const shader = get_shader_imp();
        assert_t::check_bool(shader, "shader is not valid");

        return (*shader);
    }

    auto material_base_t::get_shader_ptr() const->std::shared_ptr<shader_base_t const>
    {
        return get_shader_imp();
    }
}
