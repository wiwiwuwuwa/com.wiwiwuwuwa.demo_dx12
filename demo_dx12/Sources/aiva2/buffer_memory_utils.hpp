#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_memory.hpp>

namespace aiva2
{
    // ----------------------------------------------------

    constexpr auto from_d3d12_heap_type(D3D12_HEAP_TYPE const type)
    {
        if (type == D3D12_HEAP_TYPE_DEFAULT)
            return buffer_memory_t::GPU_ONLY;

        if (type == D3D12_HEAP_TYPE_UPLOAD)
            return buffer_memory_t::CPU_TO_GPU;

        if (type == D3D12_HEAP_TYPE_READBACK)
            return buffer_memory_t::GPU_TO_CPU;

        assert_t::check_bool(false, "unsupported d3d12 heap type");
        return buffer_memory_t::UNKNOWN;
    }

    template <D3D12_HEAP_TYPE t_type>
    constexpr auto from_d3d12_heap_type()
    {
        return from_d3d12_heap_type(t_type);
    }

    template <D3D12_HEAP_TYPE t_type>
    struct from_d3d12_heap_type_t : public std::integral_constant<buffer_memory_t, from_d3d12_heap_type(t_type)> {};

    template <D3D12_HEAP_TYPE t_type>
    constexpr auto from_d3d12_heap_type_v = from_d3d12_heap_type_t<t_type>::value;

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

    template <buffer_memory_t t_type>
    constexpr auto to_d3d12_heap_type()
    {
        return to_d3d12_heap_type(t_type);
    }

    template <buffer_memory_t t_type>
    struct to_d3d12_heap_type_t : public std::integral_constant<D3D12_HEAP_TYPE, to_d3d12_heap_type(t_type)> {};

    template <buffer_memory_t t_type>
    constexpr auto to_d3d12_heap_type_v = to_d3d12_heap_type_t<t_type>::value;
    
    // ----------------------------------------------------
}
