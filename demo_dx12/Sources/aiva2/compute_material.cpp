#include <pch.h>
#include <aiva2/compute_material.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/compute_shader.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/mat_reg.hpp>
#include <aiva2/resource_system.hpp>
#include <aiva2/shader_info.hpp>
#include <aiva2/shader_info_for_code.hpp>

namespace aiva2
{
    MAT_REG(compute_material_t);
    
    compute_material_t::compute_material_t(engine_t& engine, std::shared_ptr<compute_shader_t const> const& shader)
        : impl_type{ engine }
    {
        init_shader(shader);
        init_resources(get_shader_ref().get_info().get_code_block());
    }

    compute_material_t::compute_material_t(engine_t& engine, nlohmann::json const& json)
        : impl_type{ engine }
    {
        init_shader(json);
        init_resources(get_shader_ref().get_info().get_code_block());
    }

    compute_material_t::~compute_material_t()
    {
        shut_resources();
        shut_shader();
    }
    
    auto compute_material_t::get_shader_ref() const->compute_shader_t const&
    {
        assert_t::check_bool(m_shader, "m_shader is not valid");
        return (*m_shader);
    }

    auto compute_material_t::get_shader_ptr() const->std::shared_ptr<compute_shader_t const>
    {
        return m_shader;
    }

    void compute_material_t::init_shader(std::shared_ptr<compute_shader_t const> const& shader)
    {
        assert_t::check_bool(shader, "shader is not valid");
        m_shader = shader;
    }

    void compute_material_t::init_shader(nlohmann::json const& json)
    {
        assert_t::check_bool(!std::empty(json), "json is not valid");

        auto const shader_path = json.at("shader_path").get<std::filesystem::path>();
        assert_t::check_bool(!std::empty(shader_path), "shader_path is not valid");

        auto const shader_data = get_engine().get_resource_system().get_resource<compute_shader_t>(shader_path);
        assert_t::check_bool(shader_data, "shader_data is not valid");

        init_shader(shader_data);
    }

    void compute_material_t::shut_shader()
    {
        assert_t::check_bool(m_shader, "m_shader is not valid");
        m_shader = {};
    }
}
