#include <aiva/time_system.hpp>
#include <aiva/engine.hpp>
#include <aiva/hardware_system.hpp>

namespace aiva
{
    time_system_t::time_system_t(struct engine_t& engine)
        : impl_type{ engine }
    {
        init_timer();
    }

    time_system_t::~time_system_t()
    {
        shut_timer();
    }

    auto time_system_t::get_on_tick() -> event_action_type<>&
    {
        return m_on_tick;
    }

    void time_system_t::on_hardware_system_tick()
    {
        m_on_tick.invoke();
        m_tick_id++;
    }

    void time_system_t::init_timer()
    {
        m_tick_id = {};
        get_engine().get_hardware_system().get_on_tick().attach_listener(&this_type::on_hardware_system_tick, this);
    }

    void time_system_t::shut_timer()
    {
        get_engine().get_hardware_system().get_on_tick().detach_listener(&this_type::on_hardware_system_tick, this);
        m_tick_id = {};
    }
}
