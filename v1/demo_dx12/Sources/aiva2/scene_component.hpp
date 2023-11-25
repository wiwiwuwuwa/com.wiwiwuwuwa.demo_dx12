#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/object.hpp>

namespace aiva2
{
    struct scene_component_t : public implements_t<scene_component_t, object_t>
    {
        // ------------------------------------------------

    protected:
        scene_component_t(scene_actor_t& scene_actor);

    public:
        ~scene_component_t() override;

        // ------------------------------------------------

    public:
        auto get_actor() const -> scene_actor_t&;

        auto get_graph() const -> scene_graph_t&;

        auto get_engine() const -> engine_t&;

    private:
        scene_actor_t& m_actor;

        // ------------------------------------------------
    };
}
