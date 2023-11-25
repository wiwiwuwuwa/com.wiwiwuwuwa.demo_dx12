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

    public:
        void run() const;

        // ------------------------------------------------

    public:
        auto get_hardware_system() const -> struct hardware_system_t&;

    protected:
        void init_hardware_system();

        void shut_hardware_system();

    private:
        std::unique_ptr<struct hardware_system_t> m_hardware_system{};

        // ------------------------------------------------

    public:
        auto get_graphic_hardware() const -> struct graphic_hardware_t&;

    protected:
        void init_graphic_hardware();

        void shut_graphic_hardware();

    private:
        std::unique_ptr<struct graphic_hardware_t> m_graphic_hardware{};

        // ------------------------------------------------
    };
}
