#include <pch.h>
#include <aiva2/graphic_renderer.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/render_target.hpp>
#include <aiva2/render_color_texture_2d.hpp>
#include <aiva2/texture_2d.hpp>

namespace aiva2
{
	graphic_renderer_t::graphic_renderer_t(engine_t& engine)
		: impl_type{ engine }
	{
		init_screen_targets();
	}

	graphic_renderer_t::~graphic_renderer_t()
	{
		shut_screen_targets();
	}

	auto graphic_renderer_t::get_screen_target() const->std::shared_ptr<render_target_t> const&
	{
		auto const target_index = static_cast<size_t>(get_engine().get_graphic_hardware().get_swap_chain().GetCurrentBackBufferIndex());
		assert_t::check_bool(target_index >= size_t{} && target_index < std::size(m_screen_targets), "target_index is not valid");
		
		return m_screen_targets[target_index];
	}

	void graphic_renderer_t::init_screen_targets()
	{
		m_screen_targets = {};

		auto swap_desc = DXGI_SWAP_CHAIN_DESC1{};
		assert_t::check_hresult(get_engine().get_graphic_hardware().get_swap_chain().GetDesc1(&swap_desc), "failed to get swap_desc");

		for (auto i = std::size_t{}; i < static_cast<size_t>(swap_desc.BufferCount); i++)
		{
			auto texture_resource = winrt::com_ptr<ID3D12Resource>{};
			assert_t::check_hresult(get_engine().get_graphic_hardware().get_swap_chain().GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&texture_resource)), "failed to get texture_resource");
			assert_t::check_bool(texture_resource, "texture_resource is not valid");

			auto const& screen_target = m_screen_targets.emplace_back(std::make_shared<render_target_t>(get_engine()));
			assert_t::check_bool(screen_target, "screen_target is not valid");

			(*screen_target).add_color_texture<render_color_texture_2d_t>(texture_resource);
		}
	}

	void graphic_renderer_t::shut_screen_targets()
	{
		m_screen_targets = {};
	}
}
