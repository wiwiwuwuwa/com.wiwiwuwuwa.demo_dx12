#include <pch.h>
#include <aiva2/gpu_eye_key_utils.hpp>

#include <aiva2/gpu_eye_key.hpp>

namespace aiva2
{
    auto operator==(gpu_eye_key_t const& a, gpu_eye_key_t const& b) -> bool
    {
        auto equals = true;
        equals &= (a.type_of_resource == b.type_of_resource);
        equals &= (a.type_of_info == b.type_of_info);
        return equals;
    }

    auto hash_value(gpu_eye_key_t const& key) -> std::size_t
    {
        auto typeHash = std::size_t{};
        boost::hash_combine(typeHash, key.type_of_resource);
        boost::hash_combine(typeHash, key.type_of_info);
        return typeHash;
    }
}
