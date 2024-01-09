#include <aiva/texture_2d.hpp>

#include <aiva/assert.hpp>
#include <aiva/engine.hpp>
#include <aiva/graphic_hardware.hpp>
#include <aiva/to_dxgi_format.hpp>
#include <aiva/to_graphic_format.hpp>

namespace aiva
{
    // --------------------------------------------------------

    void texture_2d_t::create_resource_from_info(texture_2d_info_t const& in_info, Microsoft::WRL::ComPtr<ID3D12Resource>& out_resource, D3D12_RESOURCE_STATES& out_state) const
    {
        auto heap_properties = D3D12_HEAP_PROPERTIES{};
        heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
        heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heap_properties.CreationNodeMask = {};
        heap_properties.VisibleNodeMask = {};

        auto heap_flags = D3D12_HEAP_FLAG_NONE;
        if (in_info.get_support_shader_atomics()) { heap_flags |= D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS; }
        heap_flags |= D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;

        auto resource_desc = D3D12_RESOURCE_DESC{};
        resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        resource_desc.Alignment = {};
        resource_desc.Width = static_cast<UINT64>(in_info.get_width());
        resource_desc.Height = static_cast<UINT>(in_info.get_height());
        resource_desc.DepthOrArraySize = 1;
        resource_desc.MipLevels = in_info.get_support_mip_mappping() ? 0 : 1;
        resource_desc.Format = to_dxgi_format(in_info.get_format());
        resource_desc.SampleDesc.Count = 1;
        resource_desc.SampleDesc.Quality = 0;
        resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
        if (in_info.get_support_render_target()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; }
        if (in_info.get_support_depth_stencil()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; }
        if (in_info.get_support_unordered_access()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS; }
        if (!in_info.get_support_shader_resource()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE; }

        assert_t::check_hresult(get_engine().get_graphic_hardware().get_device().CreateCommittedResource
        (
            /* pHeapProperties */ &heap_properties,
            /* HeapFlags */ heap_flags,
            /* pDesc */ &resource_desc,
            /* InitialResourceState */ D3D12_RESOURCE_STATE_COMMON,
            /* pOptimizedClearValue */ nullptr,
            /* Resource */ IID_PPV_ARGS(&out_resource)
        ), "failed to create resource");

        out_state = D3D12_RESOURCE_STATE_COMMON;
    }

    void texture_2d_t::create_info_from_resource(ID3D12Resource& in_resource, texture_2d_info_t& out_info, D3D12_RESOURCE_STATES& out_state) const
    {
        auto heap_properties = D3D12_HEAP_PROPERTIES{};
        auto heap_flags = D3D12_HEAP_FLAG_NONE;
        assert_t::check_hresult(in_resource.GetHeapProperties(&heap_properties, &heap_flags));

        out_info.set_support_shader_atomics(heap_flags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS);

        auto const resource_desc = in_resource.GetDesc();
        assert_t::check_bool(resource_desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D, "resource is not a 2d texture");

        out_info.set_format(to_graphic_format(resource_desc.Format));
        out_info.set_width(static_cast<size_t>(resource_desc.Width));
        out_info.set_height(static_cast<size_t>(resource_desc.Height));
        out_info.set_support_mip_mappping(resource_desc.MipLevels > 1);
        out_info.set_support_shader_resource(!(resource_desc.Flags & D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE));
        out_info.set_support_render_target(resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
        out_info.set_support_depth_stencil(resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
        out_info.set_support_unordered_access(resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

        out_state = D3D12_RESOURCE_STATE_COMMON;
    }

    // --------------------------------------------------------
} // namespace aiva
