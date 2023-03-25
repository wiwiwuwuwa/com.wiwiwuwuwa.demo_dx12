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
			auto rtv_eye_barriers = init_rtv_eye_for_rendering();
			std::move(std::begin(rtv_eye_barriers), std::end(rtv_eye_barriers), std::back_inserter(barriers));
		}

		{
			auto dsv_eye_barriers = init_dsv_eye_for_rendering();
			std::move(std::begin(dsv_eye_barriers), std::end(dsv_eye_barriers), std::back_inserter(barriers));
		}

		return barriers;
	}

	auto render_target_t::shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		{
			auto rtv_eye_barriers = shut_rtv_eye_for_rendering();
			std::move(std::begin(rtv_eye_barriers), std::end(rtv_eye_barriers), std::back_inserter(barriers));
		}

		{
			auto dsv_eye_barriers = shut_dsv_eye_for_rendering();
			std::move(std::begin(dsv_eye_barriers), std::end(dsv_eye_barriers), std::back_inserter(barriers));
		}

		return barriers;
	}

	void render_target_t::add_rtv_eye(std::shared_ptr<rtv_eye_t> const& rtv_eye)
	{
		assert_t::check_bool(rtv_eye, "rtv_eye is not valid");
		m_rtv_eyes.push_back(rtv_eye);
		upd_rtv_eye_handle();
	}

	auto render_target_t::get_rtv_eye(size_t const index) const->std::shared_ptr<rtv_eye_t> const&
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_rtv_eyes), "index is out of range");
		return m_rtv_eyes[index];
	}

	auto render_target_t::num_rtv_eye() const->size_t
	{
		return std::size(m_rtv_eyes);
	}

	void render_target_t::rem_rtv_eye(size_t const index)
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_rtv_eyes), "index is out of range");
		m_rtv_eyes.erase(std::next(std::cbegin(m_rtv_eyes), index));
		upd_rtv_eye_handle();
	}
	
	void render_target_t::set_rtv_eye(size_t const index, std::shared_ptr<rtv_eye_t> const& rtv_eye)
	{
		assert_t::check_bool(index >= 0 && index < std::size(m_rtv_eyes), "index is out of range");
		assert_t::check_bool(rtv_eye, "rtv_eye is not valid");
		m_rtv_eyes[index] =	rtv_eye;
		upd_rtv_eye_handle();
	}

	auto render_target_t::init_rtv_eye_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};
		
		for (auto const& rtv_eye : m_rtv_eyes)
		{
			assert_t::check_bool(rtv_eye, "rtv_eye is not valid");
			
			auto const rtv_eye_barriers = (*rtv_eye).init_for_rendering();
			std::move(std::begin(rtv_eye_barriers), std::end(rtv_eye_barriers), std::back_inserter(barriers));
		}
		
		return barriers;
	}

	auto render_target_t::shut_rtv_eye_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};
		
		for (auto const& rtv_eye : m_rtv_eyes)
		{
			assert_t::check_bool(rtv_eye, "rtv_eye is not valid");

			auto const rtv_eye_barriers = (*rtv_eye).shut_for_rendering();
			std::move(std::begin(rtv_eye_barriers), std::end(rtv_eye_barriers), std::back_inserter(barriers));
		}
		
		return barriers;
	}

	auto render_target_t::get_rtv_eye_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		if (m_rtv_eye_heap)
		{
			return (*m_rtv_eye_heap).GetCPUDescriptorHandleForHeapStart();
		}
		else
		{
			return {};
		}
	}

	auto render_target_t::num_rtv_eye_handle() const->size_t
	{
		return std::size(m_rtv_eyes);
	}

	void render_target_t::upd_rtv_eye_handle()
	{
		if (std::empty(m_rtv_eyes))
		{
			m_rtv_eye_heap = {};
			return;
		}
		
		auto heap_desc = D3D12_DESCRIPTOR_HEAP_DESC{};
		heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heap_desc.NumDescriptors = static_cast<UINT>(std::size(m_rtv_eyes));
		heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heap_desc.NodeMask = {};

		assert_t::check_hresult(get_engine().get_graphic_hardware().get_device().CreateDescriptorHeap
		(
			/*pDescriptorHeapDesc*/ &heap_desc,
			/*Heap*/ IID_PPV_ARGS(&m_rtv_eye_heap)
		));
		assert_t::check_bool(m_rtv_eye_heap, "m_rtv_eye_heap is not valid");

		auto const increment_size = get_engine().get_graphic_hardware().get_device().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		for (auto i = size_t{}; i < std::size(m_rtv_eyes); ++i)
		{
			auto const& rtv_eye = m_rtv_eyes[i];
			assert_t::check_bool(rtv_eye, "rtv_eye is not valid");

			auto const bind_place = (*m_rtv_eye_heap).GetCPUDescriptorHandleForHeapStart() + i * increment_size;
			(*rtv_eye).bind_for_rendering(bind_place);
		}
	}

	auto render_target_t::get_dsv_eye() const->std::shared_ptr<dsv_eye_t> const&
	{
		return m_dsv_eye;
	}

	auto render_target_t::has_dsv_eye() const->bool
	{
		return m_dsv_eye != nullptr;
	}

	void render_target_t::set_dsv_eye(std::shared_ptr<dsv_eye_t> const& dsv_eye)
	{
		m_dsv_eye = dsv_eye;
		upd_dsv_eye_handle();
	}

	auto render_target_t::init_dsv_eye_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		if (m_dsv_eye)
		{
			barriers = (*m_dsv_eye).init_for_rendering();
		}

		return barriers;
	}

	auto render_target_t::shut_dsv_eye_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
	{
		auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		if (m_dsv_eye)
		{
			barriers = (*m_dsv_eye).shut_for_rendering();
		}

		return barriers;
	}

	auto render_target_t::get_dsv_eye_handle() const->std::optional<D3D12_CPU_DESCRIPTOR_HANDLE>
	{
		if (m_dsv_eye_heap)
		{
			return (*m_dsv_eye_heap).GetCPUDescriptorHandleForHeapStart();
		}
		else
		{
			return {};
		}
	}

	void render_target_t::upd_dsv_eye_handle()
	{
		if (!m_dsv_eye)
		{
			m_dsv_eye_heap = {};
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
			/*Heap*/ IID_PPV_ARGS(&m_dsv_eye_heap)
		));
		assert_t::check_bool(m_dsv_eye_heap, "m_dsv_eye_heap is not valid");

		auto const bind_place = (*m_dsv_eye_heap).GetCPUDescriptorHandleForHeapStart();
		(*m_dsv_eye).bind_for_rendering(bind_place);
	}
}
