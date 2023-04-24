#include <pch.h>
#include <aiva2/scene_graph_loader.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/resource_loader_registr.hpp>
#include <aiva2/resource_system.hpp>
#include <aiva2/scene_actor.hpp>
#include <aiva2/scene_gltf.hpp>
#include <aiva2/scene_graph.hpp>

namespace aiva2
{
    REGISTER_RESOURCE_LOADER(scene_graph_loader_t);

    auto scene_graph_loader_t::load(engine_t& in_engine, nlohmann::json const& in_json) -> std::shared_ptr<scene_graph_t>
    {
        auto const graph = std::make_shared<scene_graph_t>(in_engine);
        assert_t::check_bool(graph, "(graph) is not valid");

        {
            auto const gltf = load_gltf(in_engine, in_json);
            assert_t::check_bool(gltf, "(gltf) is not valid");

            auto const actors = load_actors(in_engine, (*gltf), (*graph));
            assert_t::check_bool(!std::empty(actors), "(actors) is not valid");

            load_actors_hierarchy(in_engine, (*gltf), actors);
            load_actors_transform(in_engine, (*gltf), actors);

            auto const scenes = load_scenes(in_engine, (*gltf), (*graph));
            assert_t::check_bool(!std::empty(scenes), "(scenes) is not valid");

            load_scenes_hierarchy(in_engine, (*gltf), actors, scenes);
        }

        return graph;
    }

    auto scene_graph_loader_t::load_gltf(engine_t& in_engine, nlohmann::json const& in_json) -> std::shared_ptr<scene_gltf_t>
    {
        auto const gltf_pth = in_json.at("data_path").get<std::filesystem::path>();
        assert_t::check_bool(!std::empty(gltf_pth), "(gltf_pth) is not valid");

        auto const gltf_res = in_engine.get_resource_system().get_resource<scene_gltf_t>(gltf_pth);
        assert_t::check_bool(gltf_res, "(gltf_res) is not valid");

        return gltf_res;
    }

    auto scene_graph_loader_t::load_actors(engine_t& in_engine, scene_gltf_t const& in_gltf, scene_graph_t& ref_graph) -> std::vector<std::shared_ptr<scene_actor_t>>
    {
        auto actors = std::vector<std::shared_ptr<scene_actor_t>>{};
        actors.reserve(std::size(in_gltf.get_model().nodes));

        for (auto i = size_t{}; i < std::size(in_gltf.get_model().nodes); i++)
        {
            actors.emplace_back(ref_graph.add_actor_ptr());
        }

        return actors;
    }

    auto scene_graph_loader_t::load_actors_hierarchy(engine_t& in_engine, scene_gltf_t const& in_gltf, std::vector<std::shared_ptr<scene_actor_t>> const& ref_actors) -> void
    {
        for (auto parent_index = size_t{}; parent_index < std::size(in_gltf.get_model().nodes); parent_index++)
        {
            for (auto const child_index : in_gltf.get_model().nodes[parent_index].children)
            {
                auto const& parent_actor = ref_actors.at(parent_index);
                assert_t::check_bool(parent_actor, "(parent_actor) is not valid");

                auto const& child_actor = ref_actors.at(child_index);
                assert_t::check_bool(child_actor, "(child_actor) is not valid");

                (*child_actor).set_parent(parent_actor);
            }
        }
    }

    auto scene_graph_loader_t::load_actors_transform(engine_t& in_engine, scene_gltf_t const& in_gltf, std::vector<std::shared_ptr<scene_actor_t>> const& ref_actors) -> void
    {
        for (auto i = size_t{}; i < std::size(in_gltf.get_model().nodes); i++)
        {
            auto const& gl_actor = in_gltf.get_model().nodes.at(i);
            auto const& my_actor = ref_actors.at(i);
            assert_t::check_bool(my_actor, "(my_actor) is not valid");

            if (!std::empty(gl_actor.translation))
            {
                assert_t::check_bool(std::size(gl_actor.translation) == 3, "(gl_actor.translation) is not valid");

                auto const position = glm::make_vec3(std::data(gl_actor.translation));
                (*my_actor).set_local_position(position);
            }

            if (!std::empty(gl_actor.rotation))
            {
                assert_t::check_bool(std::size(gl_actor.rotation) == 4, "(gl_actor.rotation) is not valid");

                auto const rotation = glm::make_quat(std::data(gl_actor.rotation));
                (*my_actor).set_local_rotation(rotation);
            }

            if (!std::empty(gl_actor.scale))
            {
                assert_t::check_bool(std::size(gl_actor.scale) == 3, "(gl_actor.scale) is not valid");

                auto const scale = glm::make_vec3(std::data(gl_actor.scale));
                (*my_actor).set_local_scale(scale);
            }

            if (!std::empty(gl_actor.matrix))
            {
                assert_t::check_bool(std::size(gl_actor.matrix) == 16, "(gl_actor.matrix) is not valid");

                auto const matrix = glm::make_mat4(std::data(gl_actor.matrix));
                (*my_actor).set_local_transform(matrix);
            }
        }
    }

    auto scene_graph_loader_t::load_scenes(engine_t& in_engine, scene_gltf_t const& in_gltf, scene_graph_t& ref_graph) -> std::vector<std::shared_ptr<scene_actor_t>>
    {
        auto scenes = std::vector<std::shared_ptr<scene_actor_t>>{};
        scenes.reserve(std::size(in_gltf.get_model().scenes));

        for (auto const& gl_scene : in_gltf.get_model().scenes)
        {
            scenes.emplace_back(ref_graph.add_actor_ptr());
        }

        return scenes;
    }

    auto scene_graph_loader_t::load_scenes_hierarchy(engine_t& in_engine, scene_gltf_t const& in_gltf, std::vector<std::shared_ptr<scene_actor_t>> const& ref_actors, std::vector<std::shared_ptr<scene_actor_t>> const& ref_scenes) -> void
    {
        for (auto scene_index = size_t{}; scene_index < std::size(in_gltf.get_model().scenes); scene_index++)
        {
            auto const& gl_scene = in_gltf.get_model().scenes.at(scene_index);
            auto const& my_scene = ref_scenes.at(scene_index);
            assert_t::check_bool(my_scene, "(my_scene) is not valid");

            for (auto const actor_index : gl_scene.nodes)
            {
                auto const& my_actor = ref_actors.at(actor_index);
                assert_t::check_bool(my_actor, "(my_actor) is not valid");

                (*my_actor).set_parent(my_scene);
            }
        }
    }
}
