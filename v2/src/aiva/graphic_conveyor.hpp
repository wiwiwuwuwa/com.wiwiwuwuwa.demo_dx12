#pragma once
#include <aiva/engine_object.hpp>

namespace aiva
{
    struct graphic_conveyor_t : public implements_t<graphic_conveyor_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        graphic_conveyor_t(engine_t& engine);

        ~graphic_conveyor_t() override;

        // ------------------------------------------------

    public:
        auto get_on_populate_commands() -> event_action_type<>&;

    private:
        void init_conveyor();

        void shut_conveyor();

    private:
        void on_time_system_draw();

    private:
        void wait_frame();

        void present_frame();

        void reset_command_list();

        void populate_command_list();

        void close_command_list();

        void execute_command_list();

        void execute_signal_for_frame();

    private:
        event_action_type<> m_on_populate_commands{};

        // ------------------------------------------------
    };
}
