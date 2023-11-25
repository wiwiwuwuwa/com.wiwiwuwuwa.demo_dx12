#include <aiva/hardware_system.hpp>

#include <SDL3/SDL.h>
#include <aiva/assert.hpp>

namespace aiva
{
    hardware_system_t::hardware_system_t(struct engine_t& engine)
        : impl_type{ engine }
    {
        SDL_Init(SDL_INIT_VIDEO);

        auto *const window = SDL_CreateWindow("aiva", 800, 600, NULL);
        assert_t::check_bool(window, "(window) is not valid");

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

        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    hardware_system_t::~hardware_system_t()
    {

    }
}
