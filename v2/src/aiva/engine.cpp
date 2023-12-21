#include <aiva/engine.hpp>

#include <aiva/assert.hpp>
#include <aiva/graphic_hardware.hpp>
#include <aiva/hardware_system.hpp>
#include <aiva/time_system.hpp>

namespace aiva
{
    engine_t::engine_t()
    {
        init_hardware_system();
        init_graphic_hardware();
        init_time_system();
    }

    engine_t::~engine_t()
    {
        shut_time_system();
        shut_graphic_hardware();
        shut_hardware_system();
    }

    void engine_t::run() const
    {
        assert_t::check_bool(m_hardware_system, "(m_hardware_system) is not valid");
        (*m_hardware_system).run();
    }

    auto engine_t::get_hardware_system() const -> struct hardware_system_t&
    {
        assert_t::check_bool(m_hardware_system, "(m_hardware_system) is not valid");
        return (*m_hardware_system);
    }

    void engine_t::init_hardware_system()
    {
        assert_t::check_bool(!m_hardware_system, "(m_hardware_system) is already valid");
        m_hardware_system = std::make_unique<hardware_system_t>(*this);
    }

    void engine_t::shut_hardware_system()
    {
        assert_t::check_bool(m_hardware_system, "(m_hardware_system) is not valid");
        m_hardware_system = {};
    }

    auto engine_t::get_graphic_hardware() const -> struct graphic_hardware_t&
    {
        assert_t::check_bool(m_graphic_hardware, "(m_graphic_hardware) is not valid");
        return (*m_graphic_hardware);
    }

    void engine_t::init_graphic_hardware()
    {
        assert_t::check_bool(!m_graphic_hardware, "(m_graphic_hardware) is already valid");
        m_graphic_hardware = std::make_unique<graphic_hardware_t>(*this);
    }

    void engine_t::shut_graphic_hardware()
    {
        assert_t::check_bool(m_graphic_hardware, "(m_graphic_hardware) is not valid");
        m_graphic_hardware = {};
    }

    auto engine_t::get_time_system() const -> struct time_system_t&
    {
        assert_t::check_bool(m_time_system, "(m_time_system) is not valid");
        return (*m_time_system);
    }

    void engine_t::init_time_system()
    {
        assert_t::check_bool(!m_time_system, "(m_time_system) is already valid");
        m_time_system = std::make_unique<time_system_t>(*this);
    }

    void engine_t::shut_time_system()
    {
        assert_t::check_bool(m_time_system, "(m_time_system) is not valid");
        m_time_system = {};
    }
}
