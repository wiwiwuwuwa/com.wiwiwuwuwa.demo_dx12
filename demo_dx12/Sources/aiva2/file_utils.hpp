#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    struct file_utils_t final
    {
        // ------------------------------------------------

    private:
        file_utils_t() = delete;

        // ------------------------------------------------

    public:
        static auto load_data_from_file(std::filesystem::path const& resource_path) -> std::vector<std::byte>;

        static auto load_json_from_file(std::filesystem::path const& resource_path) -> nlohmann::json;

        // ------------------------------------------------
    };
}
