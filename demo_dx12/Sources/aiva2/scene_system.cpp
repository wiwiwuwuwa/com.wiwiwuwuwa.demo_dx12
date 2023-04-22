#include <pch.h>
#include <aiva2/scene_system.hpp>

#include <aiva2/engine.hpp>

namespace aiva2
{
    scene_system_t::scene_system_t(engine_t& engine)
        : impl_type{ engine }
    {

    }

    scene_system_t::~scene_system_t()
    {

    }
}
