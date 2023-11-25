#include <pch.h>
#include <aiva2/scene_system.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/resource_system.hpp>
#include <aiva2/scene_graph.hpp>

namespace aiva2
{
    scene_system_t::scene_system_t(engine_t& engine)
        : impl_type{ engine }
    {
        init_scene();
    }

    scene_system_t::~scene_system_t()
    {
        shut_scene();
    }

    void scene_system_t::init_scene()
    {
        m_scene = get_engine().get_resource_system().get_resource<scene_graph_t>("resources2/scenes/main.json");
        assert_t::check_bool(m_scene, "(m_scene) is not valid");
    }

    void scene_system_t::shut_scene()
    {
        assert_t::check_bool(m_scene, "(m_scene) is not valid");
        m_scene = {};
    }
}
