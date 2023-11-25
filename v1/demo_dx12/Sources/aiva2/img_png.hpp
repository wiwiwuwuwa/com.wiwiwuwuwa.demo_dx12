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

    private:
        void init_self(boost::span<std::byte const> const& binary);

        void shut_self();
        
        // ------------------------------------------------

    public:
        auto get_width() const -> size_t;

    private:
        size_t m_width{};

        // ------------------------------------------------

    public:
        auto get_height() const -> size_t;

    private:
        size_t m_height{};

        // ------------------------------------------------

    public:
        auto get_channels() const -> size_t;

    private:
        size_t m_channels{};

        // ------------------------------------------------

    public:
        auto get_data() const -> boost::span<std::byte const>;

    private:
        std::vector<std::byte> m_data{};

        // ------------------------------------------------
    };
}
