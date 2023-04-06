#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    constexpr auto align_up(std::size_t const value, std::size_t const alignment) -> std::size_t
    {
        return (value + alignment - 1) & ~(alignment - 1);
    }
}
