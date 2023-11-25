#pragma once
#include <SDL3/SDL.h>
#include <aiva/engine_object.hpp>

namespace aiva
{
    struct hardware_system_t final : public implements_t<hardware_system_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        hardware_system_t(struct engine_t& engine);

        ~hardware_system_t() override;

    public:
        void run() const;

        // ------------------------------------------------

    private:
        void init_video();

        void shut_video();

    private:
        SDL_Window* m_window{};

        // ------------------------------------------------
    };
}
