#include <pch.h>
#include <aiva2/native/graphic_pipeline.hpp>

#include <aiva2/core/asserts.hpp>
#include <aiva2/native/engine.hpp>
#include <aiva2/native/graphic_hardware.hpp>
#include <aiva2/native/time_system.hpp>

namespace aiva2::native
{
	graphic_pipeline_t::graphic_pipeline_t(engine_t& engine) : m_engine{ engine }
	{
		init_graphic_pipeline();
	}

	graphic_pipeline_t::~graphic_pipeline_t()
	{
		shut_graphic_pipeline();
	}

	auto graphic_pipeline_t::get_engine() const -> engine_t&
	{
		return m_engine;
	}

	auto graphic_pipeline_t::get_on_populate_commands() -> event_action_type<>&
	{
		return m_on_populate_commands;
	}

	void graphic_pipeline_t::init_graphic_pipeline()
	{
		get_engine().get_time_system().get_on_render().attach_listener(&this_type::tick_graphic_pipeline, this);
	}

	void graphic_pipeline_t::tick_graphic_pipeline()
	{
		wait_frame();
		present_frame();
		reset_command_list();
		populate_command_list();
		close_command_list();
		execute_command_list();
		execute_signal_for_frame();
	}

	void graphic_pipeline_t::shut_graphic_pipeline()
	{
		get_engine().get_time_system().get_on_render().remove_listener(&this_type::tick_graphic_pipeline, this);
	}

	void graphic_pipeline_t::wait_frame()
	{
		auto& fence = get_engine().get_graphic_hardware().get_fence();
		
		auto const desired_frame = get_engine().get_time_system().get_tick();
		auto const current_frame = fence.GetCompletedValue();

		core::asserts_t::check_true(current_frame != UINT64_MAX, "current frame is not valid");

		if (current_frame >= desired_frame)
		{
			return;
		}
		
		winrt::check_hresult(fence.SetEventOnCompletion(desired_frame, {}));
	}

	void graphic_pipeline_t::present_frame()
	{
		static constexpr const auto IS_VSYNC_ENABLED = bool{};

		auto& graphic_hardware = get_engine().get_graphic_hardware();
		
		auto const sync_interval = IS_VSYNC_ENABLED ? 1u : 0u;
		auto const present_flags = graphic_hardware.get_is_tearing_allowed() && !IS_VSYNC_ENABLED ? DXGI_PRESENT_ALLOW_TEARING : 0u;
		
		winrt::check_hresult(graphic_hardware.get_swap_chain().Present(sync_interval, present_flags));
	}

	void graphic_pipeline_t::reset_command_list()
	{
		auto& graphic_hardware = get_engine().get_graphic_hardware();
		auto& command_allocator = graphic_hardware.get_command_allocator();
		auto& command_list = graphic_hardware.get_command_list();

		winrt::check_hresult(command_allocator.Reset());
		winrt::check_hresult(command_list.Reset(&command_allocator, nullptr));
	}

	void graphic_pipeline_t::populate_command_list()
	{
		get_on_populate_commands().broadcast();
	}

	void graphic_pipeline_t::close_command_list()
	{
		get_engine().get_graphic_hardware().get_command_list().Close();
	}

	void graphic_pipeline_t::execute_command_list()
	{
		auto& graphic_hardware = get_engine().get_graphic_hardware();
		auto& command_list = graphic_hardware.get_command_list();
		auto& command_queue = graphic_hardware.get_command_queue();

		auto const commands_list = std::array<ID3D12CommandList*, 1> { &command_list };
		command_queue.ExecuteCommandLists(1, std::data(commands_list));
	}

	void graphic_pipeline_t::execute_signal_for_frame()
	{
		auto& graphic_hardware = get_engine().get_graphic_hardware();
		auto& command_queue = graphic_hardware.get_command_queue();
		auto& fence = graphic_hardware.get_fence();

		auto const desired_frame = get_engine().get_time_system().get_tick() + 1;
		winrt::check_hresult(command_queue.Signal(&fence, desired_frame));
	}
}
