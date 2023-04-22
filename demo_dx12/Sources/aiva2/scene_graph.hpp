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

        scene_graph_t(engine_t& engine, nlohmann::json const& json);

        ~scene_graph_t() override;

        // ------------------------------------------------

    public:
        auto add_actor_ptr() -> std::shared_ptr<scene_actor_t>;

        auto add_actor_ref() -> scene_actor_t&;

        auto get_actor_ptr(size_t const index) -> std::shared_ptr<scene_actor_t>;

        auto get_actor_ref(size_t const index) -> scene_actor_t&;

        auto num_actor() const -> size_t;

        auto rem_actor(size_t const index) -> void;

    private:
        std::vector<std::shared_ptr<scene_actor_t>> m_actors{};

        // ------------------------------------------------
    };
}
