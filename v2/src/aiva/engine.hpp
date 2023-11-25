#pragma once
#include <memory>
#include <aiva/object.hpp>

namespace aiva
{
    struct engine_t final : public implements_t<engine_t, object_t>
    {
        // ------------------------------------------------

    public:
        engine_t();

        ~engine_t() override;

        // ------------------------------------------------

    public:
        auto get_hardware_system() const -> struct hardware_system_t&;

    protected:
        void init_hardware_system();

        void shut_hardware_system();

    private:
        std::unique_ptr<struct hardware_system_t> m_hardware_system{};

        // ------------------------------------------------
    };
}
