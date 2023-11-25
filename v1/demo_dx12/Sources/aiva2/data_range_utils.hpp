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

    void from_d3d12_range(D3D12_RANGE const& in_val, data_range_t& out_val)
    {
        out_val = {};
        out_val.set_offset(static_cast<size_t>(in_val.Begin));
        out_val.set_size(static_cast<size_t>(in_val.End - in_val.Begin));
    }

    // ----------------------------------------------------    
}
