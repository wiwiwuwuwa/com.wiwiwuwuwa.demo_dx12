#include <pch.h>
#include <aiva2/native/gr_texture_2d.hpp>

#include <aiva2/core/asserts.hpp>
#include <aiva2/native/buffer_format_to_dxgi_format.hpp>
#include <aiva2/native/dxgi_format_to_buffer_format.hpp>
#include <aiva2/native/engine.hpp>
#include <aiva2/native/graphic_hardware.hpp>

namespace aiva2::native
{
	gr_texture_2d_t::gr_texture_2d_t(engine_t& engine, gr_texture_2d_params_t const& params) : impl_type{ engine }, m_params{ params }
	{
		init_resource_from_params();
	}

	gr_texture_2d_t::gr_texture_2d_t(engine_t& engine, winrt::com_ptr<ID3D12Resource> const& resource) : impl_type{ engine, resource }
	{
		init_params_from_resource();
	}

	gr_texture_2d_t::~gr_texture_2d_t()
	{

	}

	auto gr_texture_2d_t::get_params() const->gr_texture_2d_params_t const&
	{
		return m_params;
	}

	void gr_texture_2d_t::init_resource_from_params()
	{
		core::asserts_t::check_true(m_params.get_format() != buffer_format_t::UNKNOWN, "texture format must be known");
		core::asserts_t::check_true(m_params.get_width() > 0, "texture width must be greater than 0");
		core::asserts_t::check_true(m_params.get_height() > 0, "texture height must be greater than 0");
		
		auto heap_properties = D3D12_HEAP_PROPERTIES{};
		heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;
		heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heap_properties.CreationNodeMask = {};
		heap_properties.VisibleNodeMask = {};

		auto heap_flags = D3D12_HEAP_FLAG_NONE;
		if (m_params.get_support_shader_atomics()) { heap_flags |= D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS; }
		heap_flags |= D3D12_HEAP_FLAG_CREATE_NOT_ZEROED;

		auto resource_desc = D3D12_RESOURCE_DESC{};
		resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		resource_desc.Alignment = {};
		resource_desc.Width = static_cast<UINT64>(m_params.get_width());
		resource_desc.Height = static_cast<UINT>(m_params.get_height());
		resource_desc.DepthOrArraySize = 1;
		resource_desc.MipLevels = m_params.get_support_mip_mappping() ? 0 : 1;
		resource_desc.Format = to_dxgi_format(m_params.get_format());
		resource_desc.SampleDesc.Count = 1;
		resource_desc.SampleDesc.Quality = 0;
		resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
		if (m_params.get_support_render_target()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; }
		if (m_params.get_support_depth_stencil()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; }
		if (m_params.get_support_unordered_access()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS; }
		if (!m_params.get_support_shader_resource()) { resource_desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE; }
		
		winrt::check_hresult(get_engine().get_graphic_hardware().get_device().CreateCommittedResource
		(
			/* pHeapProperties */ &heap_properties,
			/* HeapFlags */ heap_flags,
			/* pDesc */ &resource_desc,
			/* InitialResourceState */ D3D12_RESOURCE_STATE_COMMON,
			/* pOptimizedClearValue */ nullptr,
			/* Resource */ IID_PPV_ARGS(&m_resource)
		));
		core::asserts_t::check_true(m_resource, "resource is not valid");
	}

	void gr_texture_2d_t::init_params_from_resource()
	{
		core::asserts_t::check_true(m_resource, "resource is not valid");
		
		auto heap_properties = D3D12_HEAP_PROPERTIES{};
		auto heap_flags = D3D12_HEAP_FLAG_NONE;
		winrt::check_hresult(m_resource->GetHeapProperties(&heap_properties, &heap_flags));

		m_params.set_support_shader_atomics(heap_flags & D3D12_HEAP_FLAG_ALLOW_SHADER_ATOMICS);

		auto const resource_desc = m_resource->GetDesc();
		core::asserts_t::check_true(resource_desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D, "resource is not a 2d texture");
		
		m_params.set_format(to_buffer_format(resource_desc.Format));
		m_params.set_width(static_cast<size_t>(resource_desc.Width));
		m_params.set_height(static_cast<size_t>(resource_desc.Height));
		m_params.set_support_mip_mappping(resource_desc.MipLevels > 1);
		m_params.set_support_shader_resource(!(resource_desc.Flags & D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE));
		m_params.set_support_render_target(resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		m_params.set_support_depth_stencil(resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		m_params.set_support_unordered_access(resource_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	}
}
