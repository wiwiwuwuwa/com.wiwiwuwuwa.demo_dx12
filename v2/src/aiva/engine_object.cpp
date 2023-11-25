#include <aiva/engine_object.hpp>

#include <aiva/engine.hpp>

namespace aiva
{
    engine_object_t::engine_object_t(engine_t& engine)
        : m_engine{ engine }
    {

    }

    engine_object_t::~engine_object_t()
    {

    }

    auto engine_object_t::get_engine() const -> engine_t&
    {
        return m_engine;
    }
}
