#include <pch.h>
#include <aiva2/rtv_tex_2d.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_format_to_dxgi_format.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/rtv_eye_init_info.hpp>
#include <aiva2/tex_2d.hpp>
#include <aiva2/tex_2d_utils.hpp>

namespace aiva2
{
	rtv_tex_2d_t::rtv_tex_2d_t(engine_t& engine, std::shared_ptr<tex_2d_t> const& resource, rtv_tex_2d_info_t const& info /*= {}*/)
		: impl_type
		{
			/*engine*/ engine,
			/*init_info*/ impl_type::init_info_type{}
				.set_descriptor_heap_type(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		}
		, m_resource{ resource }
		, m_info{ info }
	{
		create_view();
	}

	rtv_tex_2d_t::~rtv_tex_2d_t()
	{
		
	}

	void rtv_tex_2d_t::init_for_rendering() const
	{
		auto const subresource_index = tex_2d_utils_t::get_subresource_index(get_resource_ref(), m_info.get_mip_slice(), m_info.get_plane_slice());
		get_resource_ref().init_state_for_transition(D3D12_RESOURCE_STATE_RENDER_TARGET, subresource_index);
		get_resource_ref().init_state_for_uav();
	}

	void rtv_tex_2d_t::shut_for_rendering() const
	{
		auto const subresource_index = tex_2d_utils_t::get_subresource_index(get_resource_ref(), m_info.get_mip_slice(), m_info.get_plane_slice());
		get_resource_ref().init_state_for_transition(D3D12_RESOURCE_STATE_COMMON, subresource_index);
		get_resource_ref().init_state_for_uav();
	}

	void rtv_tex_2d_t::create_view() const
	{
		auto view_desc = D3D12_RENDER_TARGET_VIEW_DESC{};
		view_desc.Format = to_dxgi_format(m_info.get_format());
		view_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MipSlice = static_cast<UINT>(m_info.get_mip_slice());
		view_desc.Texture2D.PlaneSlice = static_cast<UINT>(m_info.get_plane_slice());

		get_engine().get_graphic_hardware().get_device().CreateRenderTargetView
		(
			/*pResource*/ get_resource_ref().get_resource().get(),
			/*pDesc*/ &view_desc,
			/*DestDescriptor*/ get_cpu_descriptor_handle()
		);
	}

	auto rtv_tex_2d_t::get_resource_ptr() const->std::shared_ptr<tex_2d_t> const&
	{
		return m_resource;
	}

	auto rtv_tex_2d_t::get_resource_ref() const->tex_2d_t&
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");
		return (*m_resource);
	}

	auto rtv_tex_2d_t::get_info() const->rtv_tex_2d_info_t const&
	{
		return m_info;
	}
}
