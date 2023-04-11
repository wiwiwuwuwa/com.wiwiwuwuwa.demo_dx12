#include <pch.h>
#include <aiva2/tex_2d.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buf.hpp>
#include <aiva2/buffer_format_to_dxgi_format.hpp>
#include <aiva2/d3d12_resource_desc_utils.hpp>
#include <aiva2/d3d12_resource_utils.hpp>
#include <aiva2/dxgi_format_to_buffer_format.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_cmd_copy_resource.hpp>
#include <aiva2/gpu_res_reg.hpp>
#include <aiva2/graphic_executor.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/img_png.hpp>
#include <aiva2/resource_system.hpp>
#include <aiva2/tex_2d_utils.hpp>

namespace aiva2
{
	GPU_RES_REG(tex_2d_t);

	tex_2d_t::tex_2d_t(engine_t& engine, tex_2d_info_t const& info)
		: impl_type{ engine }
	{
		m_info = info;
		init_resource_from_info();
	}

	tex_2d_t::tex_2d_t(engine_t& engine, winrt::com_ptr<ID3D12Resource> const& resource)
		: impl_type{ engine }
	{
		m_resource = resource;
		init_info_from_resource();
	}

	tex_2d_t::~tex_2d_t()
	{

	}

	auto tex_2d_t::get_info() const->tex_2d_info_t const&
	{
		return m_info;
	}

	void tex_2d_t::init_resource_from_info()
	{
		assert_t::check_bool(m_info.get_format() != buffer_format_t::UNKNOWN, "texture format must be known");
		assert_t::check_bool(m_info.get_width() > 0, "texture width must be greater than 0");
		assert_t::check_bool(m_info.get_height() > 0, "texture height must be greater than 0");

		auto heap_properties = D3D12_HEAP_PROPERTIES{};
		heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_properties.CreationNodeMask = {};
		heap_properties.VisibleNodeMask = {};

		auto heap_flags = D3D12_HEAP_FLAG_NONE;
		if (m_info.get_support_shader_atomics()) { heap_flags |= D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS; }
		heap_flags |= D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;

		auto resource_desc = D3D12_RESOURCE_DESC{};
		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resource_desc.Alignment = {};
		resource_desc.Width = static_cast<UINT64>(m_info.get_width());
		resource_desc.Height = static_cast<UINT>(m_info.get_height());
		resource_desc.DepthOrArraySize = 1;
		resource_desc.MipLevels = m_info.get_support_mip_mappping() ? 0 : 1;
		resource_desc.Format = to_dxgi_format(m_info.get_format());
		resource_desc.SampleDesc.Count = 1;
		resource_desc.SampleDesc.Quality = 0;
		resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		if (m_info.get_support_render_target()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; }
		if (m_info.get_support_depth_stencil()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; }
		if (m_info.get_support_unordered_access()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS; }
		if (!m_info.get_support_shader_resource()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE; }

		assert_t::check_hresult(get_engine().get_graphic_hardware().get_device().CreateCommittedResource
		(
			/* pHeapProperties */ &heap_properties,
			/* HeapFlags */ heap_flags,
			/* pDesc */ &resource_desc,
			/* InitialResourceState */ D3D12_RESOURCE_STATE_COMMON,
			/* pOptimizedClearValue */ nullptr,
			/* Resource */ IID_PPV_ARGS(&m_resource)
		));
		assert_t::check_bool(m_resource, "resource is not valid");

		m_states = gpu_res_state_t{ D3D12_RESOURCE_STATE_COMMON, d3d12_resource_utils_t::get_subresources_count(*m_resource), false };
	}

	void tex_2d_t::init_info_from_resource()
	{
		assert_t::check_bool(m_resource, "resource is not valid");

		auto heap_properties = D3D12_HEAP_PROPERTIES{};
		auto heap_flags = D3D12_HEAP_FLAG_NONE;
		assert_t::check_hresult(m_resource->GetHeapProperties(&heap_properties, &heap_flags));

		m_info.set_support_shader_atomics(heap_flags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS);

		auto const resource_desc = m_resource->GetDesc();
		assert_t::check_bool(resource_desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D, "resource is not a 2d texture");

		m_info.set_format(to_buffer_format(resource_desc.Format));
		m_info.set_width(static_cast<size_t>(resource_desc.Width));
		m_info.set_height(static_cast<size_t>(resource_desc.Height));
		m_info.set_support_mip_mappping(resource_desc.MipLevels > 1);
		m_info.set_support_shader_resource(!(resource_desc.Flags & D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE));
		m_info.set_support_render_target(resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		m_info.set_support_depth_stencil(resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		m_info.set_support_unordered_access(resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		m_states = gpu_res_state_t{ D3D12_RESOURCE_STATE_COMMON, d3d12_resource_desc_utils_t::get_subresources_count(resource_desc), false };
	}
}
