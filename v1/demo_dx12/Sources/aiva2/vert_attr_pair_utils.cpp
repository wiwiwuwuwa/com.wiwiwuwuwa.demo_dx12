#include <pch.h>
#include <aiva2/vert_attr_pair_utils.hpp>

#include <aiva2/vert_attr_pair.hpp>

namespace aiva2
{
    auto operator==(vert_attr_pair_t const& a, vert_attr_pair_t const& b) -> bool
    {
        auto equals = true;
        equals &= (a.get_type() == b.get_type());
        equals &= (a.get_indx() == b.get_indx());
        return equals;
    }

    auto hash_value(vert_attr_pair_t const& key) -> std::size_t
    {
        auto hash = std::size_t{};
        boost::hash_combine(hash, key.get_type());
        boost::hash_combine(hash, key.get_indx());
        return hash;
    }
}
