#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
    auto operator==(gpu_eye_key_t const& a, gpu_eye_key_t const& b) -> bool;

    auto hash_value(gpu_eye_key_t const& key) -> std::size_t;
}
