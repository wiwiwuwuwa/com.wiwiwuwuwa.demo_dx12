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
        static auto load(engine_t& in_engine, nlohmann::json const& in_json)->std::shared_ptr<scene_graph_t>;

    private:
        static auto load_gltf(engine_t& in_engine, nlohmann::json const& in_json)->std::shared_ptr<scene_gltf_t>;

        static auto load_actors(engine_t& in_engine, scene_gltf_t const& in_gltf, scene_graph_t& ref_graph)->std::vector<std::shared_ptr<scene_actor_t>>;

        static auto load_actors_hierarchy(engine_t& in_engine, scene_gltf_t const& in_gltf, std::vector<std::shared_ptr<scene_actor_t>> const& ref_actors)->void;

        static auto load_actors_transform(engine_t& in_engine, scene_gltf_t const& in_gltf, std::vector<std::shared_ptr<scene_actor_t>> const& ref_actors)->void;

        static auto load_scenes(engine_t& in_engine, scene_gltf_t const& in_gltf, scene_graph_t& ref_graph)->std::vector<std::shared_ptr<scene_actor_t>>;

        static auto load_scenes_hierarchy(engine_t& in_engine, scene_gltf_t const& in_gltf, std::vector<std::shared_ptr<scene_actor_t>> const& ref_actors, std::vector<std::shared_ptr<scene_actor_t>> const& ref_scenes)->void;

        // ------------------------------------------------
    };
}
