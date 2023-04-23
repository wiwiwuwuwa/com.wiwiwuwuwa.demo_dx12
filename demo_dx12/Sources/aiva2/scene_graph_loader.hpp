#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    struct scene_graph_loader_t final
    {
        // ------------------------------------------------

    private:
        scene_graph_loader_t() = delete;

        // ------------------------------------------------

    public:
        static auto load(engine_t& engine, nlohmann::json const& json)->std::shared_ptr<scene_graph_t>;
    };
}
