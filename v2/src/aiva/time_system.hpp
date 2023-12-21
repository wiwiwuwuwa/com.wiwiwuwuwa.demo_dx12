#pragma once
#include <aiva/engine_object.hpp>

namespace aiva
{
    struct time_system_t final : public implements_t<time_system_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        time_system_t(struct engine_t& engine);

        ~time_system_t() override;

        // ------------------------------------------------

    public:
        auto get_on_tick() -> event_action_type<>&;

    private:
        void on_hardware_system_tick();

    private:
        void init_timer();

        void shut_timer();

    private:
        event_action_type<> m_on_tick{};

        size_t m_tick_id{};

        // ------------------------------------------------
    };
}
