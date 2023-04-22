#include <pch.h>
#include <aiva2/scene_component.hpp>

#include <aiva2/engine.hpp>
#include <aiva2/scene_actor.hpp>
#include <aiva2/scene_graph.hpp>

namespace aiva2
{
    scene_component_t::scene_component_t(scene_actor_t& scene_actor)
        : m_actor{ scene_actor }
    {

    }

    scene_component_t::~scene_component_t()
    {

    }

    auto scene_component_t::get_actor() const -> scene_actor_t&
    {
        return m_actor;
    }

    auto scene_component_t::get_graph() const -> scene_graph_t&
    {
        return get_actor().get_graph();
    }

    auto scene_component_t::get_engine() const -> engine_t&
    {
        return get_graph().get_engine();
    }
}
