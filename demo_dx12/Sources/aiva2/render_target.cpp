#include <pch.h>
#include <aiva2/render_target.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/render_texture_base.hpp>
#include <aiva2/render_texture_base.hpp>

namespace aiva2
{
	render_target_t::render_target_t(engine_t& engine)
		: impl_type{ engine }
	{

	}

	render_target_t::~render_target_t()
	{

	}

	void render_target_t::set_state_for_transition() const
	{
		for (auto const& color_texture : m_color_textures)
		{
			assert_t::check_bool(color_texture, "color_texture is not valid");
			(*color_texture).set_state_for_transition();
		}

		if (m_depth_texture)
		{
			(*m_depth_texture).set_state_for_transition();
		}
	}

	void render_target_t::set_state_for_uav() const
	{
		for (auto const& color_texture : m_color_textures)
		{
			assert_t::check_bool(color_texture, "color_texture is not valid");
			(*color_texture).set_state_for_uav();
		}

		if (m_depth_texture)
		{
			(*m_depth_texture).set_state_for_uav();
		}
	}

	void render_target_t::add_color_texture(std::shared_ptr<render_texture_base_t> const& color_texture)
	{
		assert_t::check_bool(color_texture, "color_texture is not valid");
		m_color_textures.push_back(color_texture);
	}

	auto render_target_t::get_color_texture(size_t const index) const->std::shared_ptr<render_texture_base_t> const&
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_color_textures), "index is out of range");
		return m_color_textures[index];
	}

	auto render_target_t::num_color_texture() const->size_t
	{
		return std::size(m_color_textures);
	}

	void render_target_t::rem_color_texture(size_t const index)
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_color_textures), "index is out of range");
		m_color_textures.erase(std::next(std::cbegin(m_color_textures), index));
	}
	
	void render_target_t::set_color_texture(size_t const index, std::shared_ptr<render_texture_base_t> const& color_texture)
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_color_textures), "index is out of range");
		assert_t::check_bool(color_texture, "color_texture is not valid");
		m_color_textures[index] = color_texture;
	}

	auto render_target_t::get_color_cpu_descriptor_handles() const->std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		auto handles = std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>{};
		for (auto const& color_texture : m_color_textures)
		{
			assert_t::check_bool(color_texture, "color_texture is not valid");
			handles.push_back((*color_texture).get_cpu_descriptor_handle());
		}
		return handles;
	}

	auto render_target_t::get_depth_texture() const->std::shared_ptr<render_texture_base_t> const&
	{
		return m_depth_texture;
	}

	auto render_target_t::has_depth_texture() const->bool
	{
		return m_depth_texture != nullptr;
	}

	void render_target_t::set_depth_texture(std::shared_ptr<render_texture_base_t> const& depth_texture)
	{
		m_depth_texture = depth_texture;
	}

	auto render_target_t::get_depth_cpu_descriptor_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		if (m_depth_texture)
		{
			return (*m_depth_texture).get_cpu_descriptor_handle();
		}
		else
		{
			return {};
		}
	}
}
