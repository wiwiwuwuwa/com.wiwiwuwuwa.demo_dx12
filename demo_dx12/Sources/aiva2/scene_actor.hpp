#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/object.hpp>

namespace aiva2
{
    struct scene_actor_t final : public implements_t<scene_actor_t, object_t>
    {
        // ------------------------------------------------

    public:
        friend scene_graph_t;

    protected:
        scene_actor_t(scene_graph_t& scene_graph);

        ~scene_actor_t() override;

        // ------------------------------------------------

    public:
        auto get_graph() const -> scene_graph_t&;

        auto get_engine() const -> engine_t&;

    private:
        scene_graph_t& m_graph;

        // ------------------------------------------------
    };
}
