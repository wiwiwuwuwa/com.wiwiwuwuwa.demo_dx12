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
                if (event.type == SDL_EVENT_QUIT)
                {
                    quit = true;
                }
            }
        }
    }

    void hardware_system_t::init_video()
    {
        SDL_InitSubSystem(SDL_INIT_VIDEO);
        m_window = SDL_CreateWindow("aiva", 800, 600, NULL);
        assert_t::check_bool(m_window, "(window) is not valid");
    }

    void hardware_system_t::shut_video()
    {
        assert_t::check_bool(m_window, "(window) is not valid");
        SDL_DestroyWindow(m_window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
}
