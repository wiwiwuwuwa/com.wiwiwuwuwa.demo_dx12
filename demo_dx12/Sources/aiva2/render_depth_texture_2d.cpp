#include <pch.h>
#include <aiva2/render_depth_texture_2d.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_format_to_dxgi_format.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/texture_2d.hpp>
#include <aiva2/texture_2d_utils.hpp>

namespace aiva2
{
	render_depth_texture_2d_t::render_depth_texture_2d_t(engine_t& engine, render_depth_texture_2d_info_t const& info, std::shared_ptr<texture_2d_t> const& resource)
		: impl_type{ engine }
		, m_info{ info }
		, m_resource{ resource }
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");
	}

	render_depth_texture_2d_t::render_depth_texture_2d_t(engine_t& engine, render_depth_texture_2d_info_t const& info, texture_2d_info_t const& resource)
		: render_depth_texture_2d_t
		{
			/*engine*/ engine,
			/*info*/ info,
			/*resource*/ std::make_shared<texture_2d_t>(engine, resource)
		}
	{

	}

	render_depth_texture_2d_t::render_depth_texture_2d_t(engine_t& engine, render_depth_texture_2d_info_t const& info, winrt::com_ptr<ID3D12Resource> const& resource)
		: render_depth_texture_2d_t
		{
			/*engine*/ engine,
			/*info*/ info,
			/*resource*/ std::make_shared<texture_2d_t>(engine, resource)
		}
	{

	}

	render_depth_texture_2d_t::~render_depth_texture_2d_t()
	{

	}

	auto render_depth_texture_2d_t::get_info() const->render_depth_texture_2d_info_t const&
	{
		return m_info;
	}

	auto render_depth_texture_2d_t::get_resource() const->std::shared_ptr<texture_2d_t> const&
	{
		return m_resource;
	}
	
	void render_depth_texture_2d_t::create_in_handle(D3D12_CPU_DESCRIPTOR_HANDLE const& dst_handle) const
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");

		auto desc = D3D12_DEPTH_STENCIL_VIEW_DESC{};
		desc.Format = to_dxgi_format(m_info.get_format());
		desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = D3D12_DSV_FLAG_NONE;
		desc.Texture2D.MipSlice = static_cast<UINT>(m_info.get_mip_slice());
		
		get_engine().get_graphic_hardware().get_device().CreateDepthStencilView
		(
			/*pResource*/ (*m_resource).get_resource().get(),
			/*pDesc*/ &desc,
			/*DestDescriptor*/ dst_handle
		);
	}

	void render_depth_texture_2d_t::set_state_for_transition() const
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");

		auto const subresource_index = texture_2d_utils_t::get_subresource_index((*m_resource), m_info);
		(*m_resource).set_state_for_transition(D3D12_RESOURCE_STATE_DEPTH_WRITE, subresource_index);
	}

	void render_depth_texture_2d_t::set_state_for_uav() const
	{
		assert_t::check_bool(m_resource, "m_resource is not valid");
		(*m_resource).set_state_for_uav();
	}
}
