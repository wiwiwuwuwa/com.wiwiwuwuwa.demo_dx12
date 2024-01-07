#include <aiva/graphic_conveyor.hpp>

#include <array>
#include <aiva/assert.hpp>
#include <aiva/engine.hpp>
#include <aiva/graphic_hardware.hpp>
#include <aiva/time_system.hpp>

namespace aiva
{
    graphic_conveyor_t::graphic_conveyor_t(engine_t& engine)
        : impl_type{ engine }
    {
        init_conveyor();
    }

    graphic_conveyor_t::~graphic_conveyor_t()
    {
        shut_conveyor();
    }

    auto graphic_conveyor_t::get_on_populate_commands() -> event_action_type<>&
    {
        return m_on_populate_commands;
    }

    void graphic_conveyor_t::init_conveyor()
    {
        get_engine().get_time_system().get_on_draw().attach_listener(&this_type::on_time_system_draw, this);
    }

    void graphic_conveyor_t::shut_conveyor()
    {
        get_engine().get_time_system().get_on_draw().detach_listener(&this_type::on_time_system_draw, this);
    }

    void graphic_conveyor_t::on_time_system_draw()
    {
        wait_frame();
        present_frame();
        reset_command_list();
        populate_command_list();
        close_command_list();
        execute_command_list();
        execute_signal_for_frame();
    }

    void graphic_conveyor_t::wait_frame()
    {
        auto& fence = get_engine().get_graphic_hardware().get_fence();

        auto const desired_frame = static_cast<UINT64>(get_engine().get_time_system().get_tick_id());
        auto const current_frame = fence.GetCompletedValue();

        assert_t::check_bool(current_frame != UINT64_MAX, "(current_frame) is not valid");

        if (current_frame >= desired_frame)
        {
            return;
        }

        assert_t::check_hresult(fence.SetEventOnCompletion(desired_frame, {}));
    }

    void graphic_conveyor_t::present_frame()
    {
        static constexpr auto const IS_VSYNC_ENABLED = bool{};

        auto& graphic_hardware = get_engine().get_graphic_hardware();

        auto const sync_interval = static_cast<UINT>(IS_VSYNC_ENABLED ? 1 : 0);
        auto const present_flags = static_cast<UINT>(graphic_hardware.get_is_tearing_allowed() && !IS_VSYNC_ENABLED ? DXGI_PRESENT_ALLOW_TEARING : NULL);

        assert_t::check_hresult(graphic_hardware.get_swap_chain().Present(sync_interval, present_flags));
    }

    void graphic_conveyor_t::reset_command_list()
    {
        auto& graphic_hardware = get_engine().get_graphic_hardware();
        auto& command_allocator = graphic_hardware.get_command_allocator();
        auto& command_list = graphic_hardware.get_command_list();

        assert_t::check_hresult(command_allocator.Reset());
        assert_t::check_hresult(command_list.Reset(&command_allocator, {}));
    }

    void graphic_conveyor_t::populate_command_list()
    {
        get_on_populate_commands().invoke();
    }

    void graphic_conveyor_t::close_command_list()
    {
        get_engine().get_graphic_hardware().get_command_list().Close();
    }

    void graphic_conveyor_t::execute_command_list()
    {
        auto& graphic_hardware = get_engine().get_graphic_hardware();
        auto& command_list = graphic_hardware.get_command_list();
        auto& command_queue = graphic_hardware.get_command_queue();

        auto const commands_list = std::array<ID3D12CommandList*, 1> { &command_list };
        command_queue.ExecuteCommandLists(static_cast<UINT>(std::size(commands_list)), std::data(commands_list));
    }

    void graphic_conveyor_t::execute_signal_for_frame()
    {
        auto& graphic_hardware = get_engine().get_graphic_hardware();
        auto& command_queue = graphic_hardware.get_command_queue();
        auto& fence = graphic_hardware.get_fence();

        auto const desired_frame = static_cast<UINT64>(get_engine().get_time_system().get_tick_id()) + static_cast<UINT64>(1);
        assert_t::check_hresult(command_queue.Signal(&fence, desired_frame));
    }
}
