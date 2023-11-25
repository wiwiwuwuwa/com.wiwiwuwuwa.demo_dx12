#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    auto operator==(vert_attr_pair_t const& a, vert_attr_pair_t const& b) -> bool;

    auto hash_value(vert_attr_pair_t const& key) -> std::size_t;
}
