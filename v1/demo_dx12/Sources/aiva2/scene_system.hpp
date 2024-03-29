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

    private:
        void init_scene();

        void shut_scene();

    private:
        std::shared_ptr<scene_graph_t> m_scene{};

        // ------------------------------------------------
    };
}
