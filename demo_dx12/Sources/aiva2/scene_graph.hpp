#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>

namespace aiva2
{
    struct scene_graph_t final : public implements_t<scene_graph_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        scene_graph_t(engine_t& engine);

        ~scene_graph_t() override;

        // ------------------------------------------------
    };
}
