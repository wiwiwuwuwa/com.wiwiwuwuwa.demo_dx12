#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    struct resource_loader_library_t final
    {
        // ------------------------------------------------

    private:
        resource_loader_library_t() = default;

    private:
        static auto get_instance() -> resource_loader_library_t&;

        // ------------------------------------------------

    public:
        using load_from_data_func = std::function<std::shared_ptr<engine_object_t>(engine_t&, boost::span<std::byte const> const&)>;

        using load_from_json_func = std::function<std::shared_ptr<engine_object_t>(engine_t&, nlohmann::json const&)>;

    public:
        static auto load_resource(engine_t& engine, std::type_index const& resource_type, std::filesystem::path const& resource_path) -> std::shared_ptr<engine_object_t>;

        static void register_loader(std::type_index const& resource_type, load_from_data_func const& loader_from_data);

        static void register_loader(std::type_index const& resource_type, load_from_json_func const& loader_from_json);

    private:
        static auto load_resource_from_data(engine_t& engine, std::type_index const& resource_type, std::filesystem::path const& resource_path) -> std::shared_ptr<engine_object_t>;

        static auto load_resource_from_json(engine_t& engine, std::type_index const& resource_type, std::filesystem::path const& resource_path) -> std::shared_ptr<engine_object_t>;

    private:
        std::unordered_map<std::type_index, load_from_data_func> m_loaders_from_data{};

        std::unordered_map<std::type_index, load_from_json_func> m_loaders_from_json{};

        // ------------------------------------------------
    };
}
