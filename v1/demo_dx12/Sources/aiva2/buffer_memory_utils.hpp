#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_memory.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    constexpr void from_d3d12_heap_type(D3D12_HEAP_TYPE const in_val, buffer_memory_t& out_val)
    {
        if (in_val == D3D12_HEAP_TYPE_DEFAULT)
            { out_val = buffer_memory_t::GPU_ONLY; return; }

        if (in_val == D3D12_HEAP_TYPE_UPLOAD)
            { out_val = buffer_memory_t::CPU_TO_GPU; return; }

        if (in_val == D3D12_HEAP_TYPE_READBACK)
            { out_val = buffer_memory_t::GPU_TO_CPU; return; }

        assert_t::check_bool(false, "unsupported d3d12 heap type");
        out_val = buffer_memory_t::UNKNOWN;
    }

    // ----------------------------------------------------

    constexpr auto to_d3d12_heap_type(buffer_memory_t const type)
    {
        if (type == buffer_memory_t::GPU_ONLY)
            return D3D12_HEAP_TYPE_DEFAULT;

        if (type == buffer_memory_t::CPU_TO_GPU)
            return D3D12_HEAP_TYPE_UPLOAD;

        if (type == buffer_memory_t::GPU_TO_CPU)
            return D3D12_HEAP_TYPE_READBACK;

        assert_t::check_bool(false, "unsupported buffer memory");
        return D3D12_HEAP_TYPE_DEFAULT;
    }
    
    // ----------------------------------------------------
}
