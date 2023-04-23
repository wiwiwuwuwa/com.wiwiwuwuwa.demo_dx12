#include <pch.h>
#include <aiva2/scene_graph_loader.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/resource_loader_registr.hpp>
#include <aiva2/resource_system.hpp>
#include <aiva2/scene_gltf.hpp>
#include <aiva2/scene_graph.hpp>

namespace aiva2
{
    REGISTER_RESOURCE_LOADER(scene_graph_loader_t);

    auto scene_graph_loader_t::load(engine_t& engine, nlohmann::json const& json) -> std::shared_ptr<scene_graph_t>
    {
        auto const gltf_pth = json.at("data_path").get<std::filesystem::path>();
        assert_t::check_bool(!std::empty(gltf_pth), "(gltf_pth) is not valid");

        auto const gltf_res = engine.get_resource_system().get_resource<scene_gltf_t>(gltf_pth);
        assert_t::check_bool(gltf_res, "(gltf_res) is not valid");

        assert_t::check_bool(false, "TODO: IMPLEMENT ME");
        return {};
    }
}
