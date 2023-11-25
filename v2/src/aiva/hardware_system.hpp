#pragma once
#include <aiva/engine_object.hpp>

namespace aiva
{
    struct hardware_system_t final : public implements_t<hardware_system_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        hardware_system_t(struct engine_t& engine);

        ~hardware_system_t() override;

        // ------------------------------------------------
    };
}
