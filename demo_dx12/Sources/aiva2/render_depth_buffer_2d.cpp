#include <pch.h>
#include <aiva2/render_depth_buffer_2d.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buffer_format_to_dxgi_format.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/texture_2d.hpp>
#include <aiva2/texture_2d_utils.hpp>

namespace aiva2
{
	render_depth_buffer_2d_t::render_depth_buffer_2d_t(engine_t& engine, render_depth_buffer_2d_info_t const& info, std::shared_ptr<texture_2d_t> const& resource)
		: impl_type{ engine, resource }
		, m_info{ info }
	{
		
	}

	render_depth_buffer_2d_t::render_depth_buffer_2d_t(engine_t& engine, render_depth_buffer_2d_info_t const& info, texture_2d_info_t const& resource)
		: render_depth_buffer_2d_t
		{
			/*engine*/ engine,
			/*info*/ info,
			/*resource*/ std::make_shared<texture_2d_t>(engine, resource)
		}
	{

	}

	render_depth_buffer_2d_t::render_depth_buffer_2d_t(engine_t& engine, render_depth_buffer_2d_info_t const& info, winrt::com_ptr<ID3D12Resource> const& resource)
		: render_depth_buffer_2d_t
		{
			/*engine*/ engine,
			/*info*/ info,
			/*resource*/ std::make_shared<texture_2d_t>(engine, resource)
		}
	{

	}

	render_depth_buffer_2d_t::~render_depth_buffer_2d_t()
	{

	}

	auto render_depth_buffer_2d_t::get_info() const->render_depth_buffer_2d_info_t const&
	{
		return m_info;
	}
	
	void render_depth_buffer_2d_t::create_in_handle(D3D12_CPU_DESCRIPTOR_HANDLE const& dst_handle) const
	{
		auto const& resource = get_resource();
		assert_t::check_bool(resource, "resource is not valid");

		auto desc = D3D12_DEPTH_STENCIL_VIEW_DESC{};
		desc.Format = to_dxgi_format(m_info.get_format());
		desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		desc.Flags = D3D12_DSV_FLAG_NONE;
		desc.Texture2D.MipSlice = static_cast<UINT>(m_info.get_mip_slice());
		
		get_engine().get_graphic_hardware().get_device().CreateDepthStencilView
		(
			/*pResource*/ (*resource).get_resource().get(),
			/*pDesc*/ &desc,
			/*DestDescriptor*/ dst_handle
		);
	}

	void render_depth_buffer_2d_t::set_state_for_transition() const
	{
		auto const& resource = get_resource();
		assert_t::check_bool(resource, "resource is not valid");

		auto const subresource_index = texture_2d_utils_t::get_subresource_index((*resource), m_info);
		(*resource).set_state_for_transition(D3D12_RESOURCE_STATE_DEPTH_WRITE, subresource_index);
	}

	void render_depth_buffer_2d_t::set_state_for_uav() const
	{
		auto const& resource = get_resource();
		assert_t::check_bool(resource, "resource is not valid");
		
		(*resource).set_state_for_uav();
	}
}
