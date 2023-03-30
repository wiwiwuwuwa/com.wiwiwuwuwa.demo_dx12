#include <pch.h>
#include <aiva2/img_png.hpp>

#include <aiva2/engine.hpp>

namespace aiva2
{
    img_png_t::img_png_t(engine_t& engine, boost::span<std::byte const> const& binary)
        : impl_type{ engine }
    {

    }

    img_png_t::~img_png_t()
    {

    }
}