#include <pch.h>
#include <aiva2/render_color_texture_2d.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_format_to_dxgi_format.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/texture_2d.hpp>
#include <aiva2/texture_2d_utils.hpp>

namespace aiva2
{
	render_color_texture_2d_t::render_color_texture_2d_t(engine_t& engine, render_color_texture_2d_info_t const& info, std::shared_ptr<texture_2d_t> const& resource)
		: impl_type{ engine }
		, m_info{ info }
		, m_resource{ resource }
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");
	}

	render_color_texture_2d_t::render_color_texture_2d_t(engine_t& engine, render_color_texture_2d_info_t const& info, texture_2d_info_t const& resource)
		: render_color_texture_2d_t
		{
			/*engine*/ engine,
			/*info*/ info,
			/*resource*/ std::make_shared<texture_2d_t>(engine, resource)
		}
	{
		
	}

	render_color_texture_2d_t::render_color_texture_2d_t(engine_t& engine, render_color_texture_2d_info_t const& info, winrt::com_ptr<ID3D12Resource> const& resource)
		: render_color_texture_2d_t
		{
			/*engine*/ engine,
			/*info*/ info,
			/*resource*/ std::make_shared<texture_2d_t>(engine, resource)
		}
	{
		
	}

	render_color_texture_2d_t::~render_color_texture_2d_t()
	{

	}

	auto render_color_texture_2d_t::get_info() const->render_color_texture_2d_info_t const&
	{
		return m_info;
	}
	
	auto render_color_texture_2d_t::get_resource() const->std::shared_ptr<texture_2d_t> const&
	{
		return m_resource;
	}
	
	auto render_color_texture_2d_t::create_in_handle(D3D12_CPU_DESCRIPTOR_HANDLE const& dst_handle) const
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");

		auto desc = D3D12_RENDER_TARGET_VIEW_DESC{};
		desc.Format = to_dxgi_format(m_info.get_format());
		desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = m_info.get_mip_slice();
		desc.Texture2D.PlaneSlice = m_info.get_plane_slice();
		
		get_engine().get_graphic_hardware().get_device().CreateRenderTargetView
		(
			/*pResource*/ (*m_resource).get_resource().get(),
			/*pDesc*/ &desc,
			/*DestDescriptor*/ dst_handle
		);
	}

	auto render_color_texture_2d_t::set_state_for_transition() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");

		auto const subresource_index = texture_2d_utils_t::get_subresource_index((*m_resource), m_info);
		return (*m_resource).set_state_for_transition(D3D12_RESOURCE_STATE_RENDER_TARGET, subresource_index);
	}
	
	auto render_color_texture_2d_t::set_state_for_uav() const->D3D12_RESOURCE_BARRIER
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");
		return (*m_resource).set_state_for_uav();
	}
}
