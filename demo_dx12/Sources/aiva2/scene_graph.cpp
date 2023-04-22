#include <pch.h>
#include <aiva2/scene_graph.hpp>

#include <aiva2/engine.hpp>

namespace aiva2
{
    scene_graph_t::scene_graph_t(engine_t& engine)
        : impl_type{ engine }
    {

    }

    scene_graph_t::~scene_graph_t()
    {

    }
}
