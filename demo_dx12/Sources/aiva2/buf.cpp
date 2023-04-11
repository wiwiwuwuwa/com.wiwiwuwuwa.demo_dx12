#include <pch.h>
#include <aiva2/buf.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_memory_utils.hpp>
#include <aiva2/d3d12_resource_desc_utils.hpp>
#include <aiva2/d3d12_resource_utils.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/enum_utils.hpp>
#include <aiva2/gpu_res_reg.hpp>
#include <aiva2/graphic_hardware.hpp>

namespace aiva2
{
    GPU_RES_REG(buf_t);

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

		auto buffer_memory = buffer_memory_t{}; from_d3d12_heap_type(heap_properties.Type, buffer_memory);
		m_info.set_memory(buffer_memory);
        m_info.set_support_shader_atomics(heap_flags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS);

        auto const resource_desc = m_resource->GetDesc();
        assert_t::check_bool(resource_desc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER, "resource is not a buffer");

        m_info.set_size(static_cast<size_t>(resource_desc.Width));
        m_info.set_support_shader_resource(!(resource_desc.Flags & D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE));
        m_info.set_support_unordered_access(resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

        auto const state_locked = m_info.get_memory() != buffer_memory_t::GPU_ONLY;
        m_states = gpu_res_state_t{ D3D12_RESOURCE_STATE_COMMON, d3d12_resource_utils_t::get_subresources_count(*m_resource), state_locked };
    }

    void buf_t::set_data(boost::span<std::byte const> const& src_bin, std::optional<size_t> const& dst_pos /*= {}*/) const
    {
        assert_t::check_bool(!std::empty(src_bin), "(src_bin) is not valid");
        assert_t::check_bool(!dst_pos || (*dst_pos) < m_info.get_size(), "(dst_pos) is out of range");
        assert_t::check_bool(!dst_pos || (*dst_pos) + std::size(src_bin) <= m_info.get_size(), "(dst_pos) is out of range");
        assert_t::check_bool(dst_pos || std::size(src_bin) <= m_info.get_size(), "(src_bin) is too big");
        assert_t::check_bool(m_resource, "(m_resource) is not valid");
        assert_t::check_bool(m_info.get_memory() != buffer_memory_t::GPU_ONLY, "(m_resource) is not CPU accessible");

        auto const dst_raw_pos = dst_pos ? D3D12_RANGE{ (*dst_pos), (*dst_pos) + std::size(src_bin) } : std::optional<D3D12_RANGE>{};
        auto dst_raw_mem = std::add_pointer_t<void>{};

        assert_t::check_hresult((*m_resource).Map
        (
            /* Subresource */ {},
            /* pReadRange */ dst_raw_pos ? &(*dst_raw_pos) : nullptr,
            /* ppData */ &dst_raw_mem
        ), "failed to map resource");
        assert_t::check_bool(dst_raw_mem, "(dst_raw_mem) is not valid");

        assert_t::check_bool(memcpy_s
        (
            /* dst */ dst_raw_mem,
            /* dst_size */ dst_raw_pos ? (*dst_raw_pos).End - (*dst_raw_pos).Begin : m_info.get_size(),
            /* src */ std::data(src_bin),
            /* src_size */ std::size(src_bin)
        ) == 0, "failed to copy data");

        (*m_resource).Unmap
        (
            /* Subresource */ {},
            /* pWrittenRange */ dst_raw_pos ? &(*dst_raw_pos) : nullptr
        );
    }

    void buf_t::get_data(boost::span<std::byte> const& dst_bin, std::optional<size_t> const& src_pos /*= {}*/) const
    {
        assert_t::check_bool(!std::empty(dst_bin), "(dst_bin) is not valid");
        assert_t::check_bool(!src_pos || (*src_pos) < m_info.get_size(), "(src_pos) is out of range");
        assert_t::check_bool(!src_pos || (*src_pos) + std::size(dst_bin) <= m_info.get_size(), "(src_pos) is out of range");
        assert_t::check_bool(src_pos || std::size(dst_bin) <= m_info.get_size(), "(dst_bin) is too big");
        assert_t::check_bool(m_resource, "(m_resource) is not valid");
        assert_t::check_bool(m_info.get_memory() != buffer_memory_t::GPU_ONLY, "(m_resource) is not CPU accessible");

        auto const src_raw_pos = src_pos ? D3D12_RANGE{ (*src_pos), (*src_pos) + std::size(dst_bin) } : std::optional<D3D12_RANGE>{};
        auto src_raw_mem = std::add_pointer_t<void>{};

        assert_t::check_hresult((*m_resource).Map
        (
            /* Subresource */ {},
            /* pReadRange */ src_raw_pos ? &(*src_raw_pos) : nullptr,
            /* ppData */ &src_raw_mem
        ), "failed to map resource");
        assert_t::check_bool(src_raw_mem, "(src_raw_mem) is not valid");

        assert_t::check_bool(memcpy_s
        (
            /* dst */ std::data(dst_bin),
            /* dst_size */ std::size(dst_bin),
            /* src */ src_raw_mem,
            /* src_size */ src_raw_pos ? (*src_raw_pos).End - (*src_raw_pos).Begin : m_info.get_size()
        ) == 0, "failed to copy data");

        (*m_resource).Unmap
        (
            /* Subresource */ {},
            /* pWrittenRange */ src_raw_pos ? &(*src_raw_pos) : nullptr
        );
    }
}
