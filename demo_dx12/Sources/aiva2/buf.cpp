#include <pch.h>
#include <aiva2/buf.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_memory_utils.hpp>
#include <aiva2/d3d12_resource_desc_utils.hpp>
#include <aiva2/d3d12_resource_utils.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/enum_utils.hpp>
#include <aiva2/graphic_hardware.hpp>

namespace aiva2
{
    buf_t::buf_t(engine_t& engine, buf_info_t const& info)
        : impl_type{ engine }
    {
        m_info = info;
        init_resource_from_info();
    }

    buf_t::buf_t(engine_t& engine, winrt::com_ptr<ID3D12Resource> const& resource)
        : impl_type{ engine }
    {
        m_resource = resource;
        init_info_from_resource();
    }

    buf_t::~buf_t()
    {

    }

    auto buf_t::get_info() const->buf_info_t const&
    {
        return m_info;
    }

    void buf_t::init_resource_from_info()
    {
        assert_t::check_bool(is_valid(m_info.get_memory()), "invalid (m_info.m_memory)");
        assert_t::check_bool(m_info.get_size() > size_t{}, "invalid (m_info.m_size)");

        auto heap_properties = D3D12_HEAP_PROPERTIES{};
        heap_properties.Type = to_d3d12_heap_type(m_info.get_memory());
        heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heap_properties.CreationNodeMask = {};
        heap_properties.VisibleNodeMask = {};

        auto heap_flags = D3D12_HEAP_FLAG_NONE;
        if (m_info.get_support_shader_atomics()) { heap_flags |= D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS; }
        heap_flags |= D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;
        heap_flags |= D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;

        auto resource_desc = D3D12_RESOURCE_DESC{};
        resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        resource_desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
        resource_desc.Width = static_cast<UINT64>(m_info.get_size());
        resource_desc.Height = 1;
        resource_desc.DepthOrArraySize = 1;
        resource_desc.MipLevels = 1;
        resource_desc.Format = DXGI_FORMAT_UNKNOWN;
        resource_desc.SampleDesc.Count = 1;
        resource_desc.SampleDesc.Quality = 0;
        resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
        if (m_info.get_support_unordered_access()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS; }
        if (!m_info.get_support_shader_resource()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE; }

        auto resource_state = D3D12_RESOURCE_STATE_COMMON;
        if (m_info.get_memory() == buffer_memory_t::CPU_TO_GPU) { resource_state = D3D12_RESOURCE_STATE_GENERIC_READ; }
        if (m_info.get_memory() == buffer_memory_t::GPU_TO_CPU) { resource_state = D3D12_RESOURCE_STATE_COPY_DEST; }

        assert_t::check_hresult(get_engine().get_graphic_hardware().get_device().CreateCommittedResource
        (
            /* pHeapProperties */ &heap_properties,
            /* HeapFlags */ heap_flags,
            /* pDesc */ &resource_desc,
            /* InitialResourceState */ resource_state,
            /* pOptimizedClearValue */ nullptr,
            /* Resource */ IID_PPV_ARGS(&m_resource)
        ));
        assert_t::check_bool(m_resource, "resource is not valid");

        auto const state_locked = m_info.get_memory() != buffer_memory_t::GPU_ONLY;
        m_states = gpu_res_state_t{ resource_state, d3d12_resource_utils_t::get_subresources_count(*m_resource), state_locked };
    }

    void buf_t::init_info_from_resource()
    {
        assert_t::check_bool(m_resource, "resource is not valid");

        auto heap_properties = D3D12_HEAP_PROPERTIES{};
        auto heap_flags = D3D12_HEAP_FLAG_NONE;
        assert_t::check_hresult(m_resource->GetHeapProperties(&heap_properties, &heap_flags));

        m_info.set_memory(from_d3d12_heap_type(heap_properties.Type));
        m_info.set_support_shader_atomics(heap_flags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS);

        auto const resource_desc = m_resource->GetDesc();
        assert_t::check_bool(resource_desc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER, "resource is not a buffer");

        m_info.set_size(static_cast<size_t>(resource_desc.Width));
        m_info.set_support_shader_resource(!(resource_desc.Flags & D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE));
        m_info.set_support_unordered_access(resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

        auto const state_locked = m_info.get_memory() != buffer_memory_t::GPU_ONLY;
        m_states = gpu_res_state_t{ D3D12_RESOURCE_STATE_COMMON, d3d12_resource_utils_t::get_subresources_count(*m_resource), state_locked };
    }
}
