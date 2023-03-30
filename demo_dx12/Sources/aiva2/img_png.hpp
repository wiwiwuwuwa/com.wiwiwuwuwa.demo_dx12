#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>

namespace aiva2
{
    struct img_png_t final : public implements_t<img_png_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        img_png_t(engine_t& engine, boost::span<std::byte const> const& binary);

        ~img_png_t() override;

        // ------------------------------------------------
    };
}
