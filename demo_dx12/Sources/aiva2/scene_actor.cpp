#include <pch.h>
#include <aiva2/scene_actor.hpp>

#include <aiva2/engine.hpp>
#include <aiva2/scene_graph.hpp>

namespace aiva2
{
    scene_actor_t::scene_actor_t(scene_graph_t& scene_graph)
        : m_graph{ scene_graph }
    {

    }

    scene_actor_t::~scene_actor_t()
    {

    }

    auto scene_actor_t::get_graph() const -> scene_graph_t&
    {
        return m_graph;
    }

    auto scene_actor_t::get_engine() const -> engine_t&
    {
        return get_graph().get_engine();
    }
}
