#include <aiva/assert.hpp>

#include <cstdlib>
#include <SDL3/SDL.h>

namespace aiva
{
    void assert_t::abort(std::string const& message /*= {}*/)
    {
        SDL_Log("aiva > %s", message.c_str());
        SDL_TriggerBreakpoint();

        std::abort();
    }
}
