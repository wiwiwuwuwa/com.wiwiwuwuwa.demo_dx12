#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    struct string_view_utils_t final
    {
        // ------------------------------------------------

    private:
        string_view_utils_t() = delete;

    public:
        static auto to_string_view(boost::span<std::byte const> const& span) -> std::string_view;

        // ------------------------------------------------
    };
}
