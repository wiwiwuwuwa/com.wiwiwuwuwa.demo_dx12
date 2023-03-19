#include <pch.h>
#include <aiva2/uav_tex_2d.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_format_to_dxgi_format.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/texture_2d.hpp>
#include <aiva2/texture_2d_utils.hpp>

namespace aiva2
{
	uav_tex_2d_t::uav_tex_2d_t(engine_t& engine, std::shared_ptr<texture_2d_t> const& resource, uav_tex_2d_info_t const& info)
		: impl_type{ engine }
		, m_resource{ resource }
		, m_info{ info }
	{
		create_view();
	}

	uav_tex_2d_t::~uav_tex_2d_t()
	{
		
	}

	void uav_tex_2d_t::create_view() const
	{
		auto view_desc = D3D12_UNORDERED_ACCESS_VIEW_DESC{};
		view_desc.Format = to_dxgi_format(m_info.get_format());
		view_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MipSlice = static_cast<UINT>(m_info.get_mip_slice());
		view_desc.Texture2D.PlaneSlice = static_cast<UINT>(m_info.get_plane_slice());

		get_engine().get_graphic_hardware().get_device().CreateUnorderedAccessView
		(
			/*pResource*/ get_resource_ref().get_resource().get(),
			/*pCounterResource*/{},
			/*pDesc*/ &view_desc,
			/*DestDescriptor*/ get_cpu_descriptor_handle()
		);
	}

	void uav_tex_2d_t::init_for_rendering() const
	{
		auto const subresource_index = texture_2d_utils_t::get_subresource_index(get_resource_ref(), m_info.get_mip_slice(), m_info.get_plane_slice());
		get_resource_ref().init_state_for_transition(D3D12_RESOURCE_STATE_UNORDERED_ACCESS, subresource_index);
		get_resource_ref().init_state_for_uav();
	}

	void uav_tex_2d_t::shut_for_rendering() const
	{
		auto const subresource_index = texture_2d_utils_t::get_subresource_index(get_resource_ref(), m_info.get_mip_slice(), m_info.get_plane_slice());
		get_resource_ref().init_state_for_transition(D3D12_RESOURCE_STATE_COMMON, subresource_index);
		get_resource_ref().init_state_for_uav();
	}

	auto uav_tex_2d_t::get_resource_ptr() const-> std::shared_ptr<texture_2d_t> const&
	{
		return m_resource;
	}

	auto uav_tex_2d_t::get_resource_ref() const->texture_2d_t&
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");
		return (*m_resource);
	}

	auto uav_tex_2d_t::get_info() const->uav_tex_2d_info_t const&
	{
		return m_info;
	}
}
