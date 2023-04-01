#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/data_range.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    auto to_d3d12_range(data_range_t const& in_range)
    {
        auto out_range = D3D12_RANGE{};
        out_range.Begin = static_cast<SIZE_T>(in_range.get_offset());
        out_range.End = static_cast<SIZE_T>(in_range.get_offset() + in_range.get_size());
        return out_range;
    }
    
    // ----------------------------------------------------

    auto from_d3d12_range(D3D12_RANGE const& in_range)
    {
        auto out_range = data_range_t{};
        out_range.set_offset(static_cast<size_t>(in_range.Begin));
        out_range.set_size(static_cast<size_t>(in_range.End - in_range.Begin));
        return out_range;
    }

    // ----------------------------------------------------    
}
