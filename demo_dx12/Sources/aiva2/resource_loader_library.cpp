#include <pch.h>
#include <aiva2/resource_loader_library.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/engine_object.hpp>
#include <aiva2/file_utils.hpp>

namespace aiva2
{
    auto resource_loader_library_t::get_instance() -> resource_loader_library_t&
    {
        static auto instance = resource_loader_library_t{};
        return instance;
    }

    auto resource_loader_library_t::load_resource(engine_t& engine, std::type_index const& resource_type, std::filesystem::path const& resource_path) -> std::shared_ptr<engine_object_t>
    {
        assert_t::check_bool(resource_type != typeid(void), "(resource_type) is not valid");
        assert_t::check_bool(!std::empty(resource_path), "(resource_path) is not valid");

        if (auto const resource_from_json = load_resource_from_json(engine, resource_type, resource_path); resource_from_json)
            return resource_from_json;

        if (auto const resource_from_data = load_resource_from_data(engine, resource_type, resource_path); resource_from_data)
            return resource_from_data;
        
        return {};
    }

    void resource_loader_library_t::register_loader(std::type_index const& resource_type, load_from_data_func const& loader_from_data)
    {
        assert_t::check_bool(resource_type != typeid(void), "(resource_type) is not valid");
        assert_t::check_bool(loader_from_data, "(loader_from_data) is not valid");

        get_instance().m_loaders_from_data[resource_type] = loader_from_data;
    }

    void resource_loader_library_t::register_loader(std::type_index const& resource_type, load_from_json_func const& loader_from_json)
    {
        assert_t::check_bool(resource_type != typeid(void), "(resource_type) is not valid");
        assert_t::check_bool(loader_from_json, "(loader_from_json) is not valid");

        get_instance().m_loaders_from_json[resource_type] = loader_from_json;
    }

    auto resource_loader_library_t::load_resource_from_data(engine_t& engine, std::type_index const& resource_type, std::filesystem::path const& resource_path) -> std::shared_ptr<engine_object_t>
    {
        assert_t::check_bool(resource_type != typeid(void), "(resource_type) is not valid");
        assert_t::check_bool(!std::empty(resource_path), "(resource_path) is not valid");

        auto const load_iter = get_instance().m_loaders_from_data.find(resource_type);
        if (load_iter == std::end(get_instance().m_loaders_from_data)) return {};

        auto const& load_func = (*load_iter).second;
        assert_t::check_bool(load_func, "(load_func) is not valid");

        auto const file_data = file_utils_t::load_data_from_file(resource_path);
        assert_t::check_bool(!std::empty(file_data), "failed to load data from file: " + resource_path.string());

        return load_func(engine, file_data);
    }

    auto resource_loader_library_t::load_resource_from_json(engine_t& engine, std::type_index const& resource_type, std::filesystem::path const& resource_path) -> std::shared_ptr<engine_object_t>
    {
        assert_t::check_bool(resource_type != typeid(void), "(resource_type) is not valid");
        assert_t::check_bool(!std::empty(resource_path), "(resource_path) is not valid");

        auto const load_iter = get_instance().m_loaders_from_json.find(resource_type);
        if (load_iter == std::end(get_instance().m_loaders_from_json)) return {};

        auto const& load_func = (*load_iter).second;
        assert_t::check_bool(load_func, "(load_func) is not valid");

        auto const file_data = file_utils_t::load_data_from_file(resource_path);
        assert_t::check_bool(!std::empty(file_data), "failed to load data from file: " + resource_path.string());

        auto const json = nlohmann::json::parse(file_data);
        assert_t::check_bool(!json.empty(), "failed to parse json from file: " + resource_path.string());

        return load_func(engine, json);
    }
}
