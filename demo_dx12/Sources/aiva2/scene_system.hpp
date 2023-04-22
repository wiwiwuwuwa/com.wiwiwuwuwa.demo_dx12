#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>

namespace aiva2
{
    struct scene_system_t final : public implements_t<scene_system_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        scene_system_t(engine_t& engine);

        ~scene_system_t() override;

        // ------------------------------------------------
    };
}
