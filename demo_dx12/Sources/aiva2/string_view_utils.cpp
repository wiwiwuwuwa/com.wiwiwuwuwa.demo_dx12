#include <pch.h>
#include <aiva2/string_view_utils.hpp>

namespace aiva2
{
    auto string_view_utils_t::to_string_view(boost::span<std::byte const> const& span) -> std::string_view
    {
        return std::string_view{ reinterpret_cast<std::string_view::const_pointer>(std::data(span)), std::size(span) };
    }
}
