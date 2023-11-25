#include <aiva/hardware_system.hpp>

#include <aiva/assert.hpp>

namespace aiva
{
    hardware_system_t::hardware_system_t(struct engine_t& engine)
        : impl_type{ engine }
    {
        init_video();
    }

    hardware_system_t::~hardware_system_t()
    {
        shut_video();
    }

    void hardware_system_t::run() const
    {
        auto quit = bool{};
        while (!quit)
        {
            auto event = SDL_Event{};
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    quit = true;
                }
            }
        }
    }

    auto hardware_system_t::get_hwnd() const -> HWND
    {
        auto info = SDL_SysWMinfo{};
        SDL_VERSION(&info.version);

        assert_t::check_bool(SDL_GetWindowWMInfo(m_window, &info), "(SDL_GetWindowWMInfo) is not valid");
        return info.info.win.window;
    }

    void hardware_system_t::init_video()
    {
        SDL_InitSubSystem(SDL_INIT_VIDEO);
        m_window = SDL_CreateWindow("aiva", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
        assert_t::check_bool(m_window, "(window) is not valid");
    }

    void hardware_system_t::shut_video()
    {
        assert_t::check_bool(m_window, "(window) is not valid");
        SDL_DestroyWindow(m_window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
}
