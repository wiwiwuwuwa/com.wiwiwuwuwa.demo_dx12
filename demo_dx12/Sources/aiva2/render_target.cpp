#include <pch.h>
#include <aiva2/render_target.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/d3d12_cpu_descriptor_handle_utils.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_eye.hpp>
#include <aiva2/gpu_eye_info.hpp>
#include <aiva2/gpu_eye_lib.hpp>
#include <aiva2/gpu_res.hpp>
#include <aiva2/graphic_hardware.hpp>

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
			auto color_barriers = init_color_resources_for_rendering();
			std::move(std::cbegin(color_barriers), std::cend(color_barriers), std::back_inserter(barriers));
		}

		{
			auto depth_barriers = init_depth_resource_for_rendering();
			std::move(std::cbegin(depth_barriers), std::cend(depth_barriers), std::back_inserter(barriers));
		}

		return barriers;
	}

	auto render_target_t::shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		{
			auto color_barriers = shut_color_resources_for_rendering();
			std::move(std::cbegin(color_barriers), std::cend(color_barriers), std::back_inserter(barriers));
		}

		{
			auto depth_barriers = shut_depth_resource_for_rendering();
			std::move(std::cbegin(depth_barriers), std::cend(depth_barriers), std::back_inserter(barriers));
		}

		return barriers;
	}

	void render_target_t::add_color_resource(std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info)
	{
		assert_t::check_bool(resource, "resource is not valid");
		assert_t::check_bool(info, "info is not valid");

		auto const color_view = gpu_eye_lib_t::new_eye(get_engine(), resource, info);
		assert_t::check_bool(color_view, "color_view is not valid");

		m_color_views.push_back(color_view);
		upd_color_resource();
	}

	auto render_target_t::num_color_resource() const->size_t
	{
		return std::size(m_color_views);
	}

	void render_target_t::rem_color_resource(size_t const index)
	{
		assert_t::check_bool(index >= size_t{}, "index is out of range");
		assert_t::check_bool(index < std::size(m_color_views), "index is out of range");

		m_color_views.erase(std::next(std::cbegin(m_color_views), index));
		upd_color_resource();
	}

	void render_target_t::set_color_resource(size_t const index, std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info)
	{
		assert_t::check_bool(index >= size_t{}, "index is out of range");
		assert_t::check_bool(index < std::size(m_color_views), "index is out of range");
		assert_t::check_bool(resource, "resource is not valid");
		assert_t::check_bool(info, "info is not valid");

		auto const color_view = gpu_eye_lib_t::new_eye(get_engine(), resource, info);
		assert_t::check_bool(color_view, "color_view is not valid");

		m_color_views[index] = color_view;
		upd_color_resource();
	}

	auto render_target_t::get_color_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		if (!m_color_heap)
		{
			return {};
		}

		return (*m_color_heap).GetCPUDescriptorHandleForHeapStart();
	}

	auto render_target_t::num_color_handle() const->size_t
	{
		if (!m_color_heap)
		{
			return {};
		}

		return static_cast<size_t>((*m_color_heap).GetDesc().NumDescriptors);
	}

	auto render_target_t::init_color_resources_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		for (auto const& color_view : m_color_views)
		{
			assert_t::check_bool(color_view, "color_view is not valid");

			auto const color_barriers = (*color_view).init_for_rendering();
			std::move(std::cbegin(color_barriers), std::cend(color_barriers), std::back_inserter(barriers));
		}

		return barriers;
	}

	auto render_target_t::shut_color_resources_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		for (auto const& color_view : m_color_views)
		{
			assert_t::check_bool(color_view, "color_view is not valid");

			auto const color_barriers = (*color_view).shut_for_rendering();
			std::move(std::cbegin(color_barriers), std::cend(color_barriers), std::back_inserter(barriers));
		}

		return barriers;
	}

	void render_target_t::upd_color_resource()
	{
		if (std::empty(m_color_views))
		{
			m_color_heap = {};
			return;
		}

		auto heap_desc = D3D12_DESCRIPTOR_HEAP_DESC{};
		heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heap_desc.NumDescriptors = static_cast<UINT>(std::size(m_color_views));
		heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heap_desc.NodeMask = {};

		assert_t::check_hresult(get_engine().get_graphic_hardware().get_device().CreateDescriptorHeap
		(
			/*pDescriptorHeapDesc*/ &heap_desc,
			/*Heap*/ IID_PPV_ARGS(&m_color_heap)
		));
		assert_t::check_bool(m_color_heap, "m_color_heap is not valid");

		auto const increment_size = get_engine().get_graphic_hardware().get_device().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		for (auto i = size_t{}; i < std::size(m_color_views); ++i)
		{
			auto const& color_view = m_color_views[i];
			assert_t::check_bool(color_view, "color_view is not valid");

			auto const bind_place = (*m_color_heap).GetCPUDescriptorHandleForHeapStart() + i * increment_size;
			(*color_view).bind_for_rendering(bind_place);
		}
	}

	auto render_target_t::has_depth_resource() const->bool
	{
		return m_depth_view != nullptr;
	}

	void render_target_t::set_depth_resource(std::shared_ptr<gpu_res_t> const& resource, std::shared_ptr<gpu_eye_info_t> const& info)
	{
		assert_t::check_bool(resource, "resource is not valid");
		assert_t::check_bool(info, "info is not valid");

		m_depth_view = gpu_eye_lib_t::new_eye(get_engine(), resource, info);
		assert_t::check_bool(m_depth_view, "m_depth_view is not valid");

		upd_depth_resource();
	}

	auto render_target_t::get_depth_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		if (!m_depth_heap)
		{
			return {};
		}

		return (*m_depth_heap).GetCPUDescriptorHandleForHeapStart();
	}

	auto render_target_t::init_depth_resource_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		if (m_depth_view)
		{
			barriers = (*m_depth_view).init_for_rendering();
		}

		return barriers;
	}

	auto render_target_t::shut_depth_resource_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		if (m_depth_view)
		{
			barriers = (*m_depth_view).shut_for_rendering();
		}

		return barriers;
	}

	void render_target_t::upd_depth_resource()
	{
		if (!m_depth_view)
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
		(*m_depth_view).bind_for_rendering(bind_place);
	}
}
