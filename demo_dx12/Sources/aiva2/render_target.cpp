#include <pch.h>
#include <aiva2/render_target.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/d3d12_cpu_descriptor_handle_utils.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/dsv_eye.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/rtv_eye.hpp>

namespace aiva2
{
	render_target_t::render_target_t(engine_t& engine)
		: impl_type{ engine }
	{

	}

	render_target_t::~render_target_t()
	{

	}

	auto render_target_t::init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		{
			auto color_texture_barriers = init_color_texture_for_rendering();
			std::move(std::begin(color_texture_barriers), std::end(color_texture_barriers), std::back_inserter(barriers));
		}

		{
			auto depth_texture_barriers = init_depth_texture_for_rendering();
			std::move(std::begin(depth_texture_barriers), std::end(depth_texture_barriers), std::back_inserter(barriers));
		}

		return barriers;
	}

	auto render_target_t::shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		{
			auto color_texture_barriers = shut_color_texture_for_rendering();
			std::move(std::begin(color_texture_barriers), std::end(color_texture_barriers), std::back_inserter(barriers));
		}

		{
			auto depth_texture_barriers = shut_depth_texture_for_rendering();
			std::move(std::begin(depth_texture_barriers), std::end(depth_texture_barriers), std::back_inserter(barriers));
		}

		return barriers;
	}

	void render_target_t::add_color_texture(std::shared_ptr<rtv_eye_t> const& color_texture)
	{
		assert_t::check_bool(color_texture, "color_texture is not valid");
		m_color_textures.push_back(color_texture);
		upd_color_texture_handle();
	}

	auto render_target_t::get_color_texture(size_t const index) const->std::shared_ptr<rtv_eye_t> const&
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
		upd_color_texture_handle();
	}
	
	void render_target_t::set_color_texture(size_t const index, std::shared_ptr<rtv_eye_t> const& color_texture)
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_color_textures), "index is out of range");
		assert_t::check_bool(color_texture, "color_texture is not valid");
		m_color_textures[index] = color_texture;
		upd_color_texture_handle();
	}

	auto render_target_t::init_color_texture_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};
		
		for (auto const& color_texture : m_color_textures)
		{
			assert_t::check_bool(color_texture, "color_texture is not valid");
			
			auto const color_barriers = (*color_texture).init_for_rendering();
			std::move(std::begin(color_barriers), std::end(color_barriers), std::back_inserter(barriers));
		}
		
		return barriers;
	}

	auto render_target_t::shut_color_texture_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};
		
		for (auto const& color_texture : m_color_textures)
		{
			assert_t::check_bool(color_texture, "color_texture is not valid");

			auto const color_barriers = (*color_texture).shut_for_rendering();
			std::move(std::begin(color_barriers), std::end(color_barriers), std::back_inserter(barriers));
		}
		
		return barriers;
	}

	auto render_target_t::get_color_texture_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		if (m_color_heap)
		{
			return (*m_color_heap).GetCPUDescriptorHandleForHeapStart();
		}
		else
		{
			return {};
		}
	}

	auto render_target_t::num_color_texture_handle() const->size_t
	{
		return std::size(m_color_textures);
	}

	void render_target_t::upd_color_texture_handle()
	{
		if (std::empty(m_color_textures))
		{
			m_color_heap = {};
			return;
		}
		
		auto heap_desc = D3D12_DESCRIPTOR_HEAP_DESC{};
		heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heap_desc.NumDescriptors = static_cast<UINT>(std::size(m_color_textures));
		heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heap_desc.NodeMask = {};

		assert_t::check_hresult(get_engine().get_graphic_hardware().get_device().CreateDescriptorHeap
		(
			/*pDescriptorHeapDesc*/ &heap_desc,
			/*Heap*/ IID_PPV_ARGS(&m_color_heap)
		));
		assert_t::check_bool(m_color_heap, "m_color_heap is not valid");

		auto const increment_size = get_engine().get_graphic_hardware().get_device().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		for (auto i = size_t{}; i < std::size(m_color_textures); ++i)
		{
			auto const& color_texture = m_color_textures[i];
			assert_t::check_bool(color_texture, "color_texture is not valid");

			auto const bind_place = (*m_color_heap).GetCPUDescriptorHandleForHeapStart() + i * increment_size;
			(*color_texture).bind_for_rendering(bind_place);
		}
	}

	auto render_target_t::get_depth_texture() const->std::shared_ptr<dsv_eye_t> const&
	{
		return m_depth_texture;
	}

	auto render_target_t::has_depth_texture() const->bool
	{
		return m_depth_texture != nullptr;
	}

	void render_target_t::set_depth_texture(std::shared_ptr<dsv_eye_t> const& depth_texture)
	{
		m_depth_texture = depth_texture;
		upd_depth_texture_handle();
	}

	auto render_target_t::init_depth_texture_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		if (m_depth_texture)
		{
			barriers = (*m_depth_texture).init_for_rendering();
		}

		return barriers;
	}

	auto render_target_t::shut_depth_texture_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		if (m_depth_texture)
		{
			barriers = (*m_depth_texture).shut_for_rendering();
		}

		return barriers;
	}

	auto render_target_t::get_depth_texture_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		if (m_depth_heap)
		{
			return (*m_depth_heap).GetCPUDescriptorHandleForHeapStart();
		}
		else
		{
			return {};
		}
	}

	void render_target_t::upd_depth_texture_handle()
	{
		if (!m_depth_texture)
		{
			m_depth_heap = {};
			return;
		}

		auto heap_desc = D3D12_DESCRIPTOR_HEAP_DESC{};
		heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heap_desc.NumDescriptors = 1;
		heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heap_desc.NodeMask = {};

		assert_t::check_hresult(get_engine().get_graphic_hardware().get_device().CreateDescriptorHeap
		(
			/*pDescriptorHeapDesc*/ &heap_desc,
			/*Heap*/ IID_PPV_ARGS(&m_depth_heap)
		));
		assert_t::check_bool(m_depth_heap, "m_depth_heap is not valid");

		auto const bind_place = (*m_depth_heap).GetCPUDescriptorHandleForHeapStart();
		(*m_depth_texture).bind_for_rendering(bind_place);
	}
}
