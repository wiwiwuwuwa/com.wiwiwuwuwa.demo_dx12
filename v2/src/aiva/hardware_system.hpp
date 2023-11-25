#pragma once
#include <windows.h>
#include <SDL.h>
#include <SDL_syswm.h>
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

    public:
        auto get_hwnd() const -> HWND;

    private:
        void init_video();

        void shut_video();

    private:
        SDL_Window* m_window{};

        // ------------------------------------------------
    };
}
