#include <pch.h>
#include <aiva2/img_png.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>

namespace aiva2
{
    img_png_t::img_png_t(engine_t& engine, boost::span<std::byte const> const& binary)
        : impl_type{ engine }
    {
        init_self(binary);
    }

    img_png_t::~img_png_t()
    {
        shut_self();
    }

    void img_png_t::init_self(boost::span<std::byte const> const& binary)
    {
        assert_t::check_bool(!std::empty(binary), "(binary) is not valid");

        auto const in_buffer = reinterpret_cast<stbi_uc const*>(std::data(binary));
        auto const in_length = static_cast<int>(std::size(binary));

        assert_t::check_bool(in_buffer, "(in_buffer) is not valid");
        assert_t::check_bool(in_length > decltype(in_length) {}, "(in_length) is not valid");

        auto out_x = int{}, out_y = int{}, out_c = int{};
        auto const out_buffer = stbi_load_from_memory(in_buffer, in_length, &out_x, &out_y, &out_c, STBI_rgb_alpha);

        assert_t::check_bool(out_buffer, "(out_buffer) is not valid");
        assert_t::check_bool(out_x > decltype(out_x) {}, "(out_x) is not valid");
        assert_t::check_bool(out_y > decltype(out_y) {}, "(out_y) is not valid");
        assert_t::check_bool(out_c > decltype(out_c) {}, "(out_c) is not valid");

        m_width = static_cast<decltype(m_width)>(out_x);
        m_height = static_cast<decltype(m_height)>(out_y);
        m_channels = static_cast<decltype(m_channels)>(STBI_rgb_alpha);
        m_data = decltype(m_data)(reinterpret_cast<std::byte*>(out_buffer), reinterpret_cast<std::byte*>(out_buffer) + out_x * out_y * out_c);

        assert_t::check_bool(m_width > decltype(m_width) {}, "(m_width) is not valid");
        assert_t::check_bool(m_height > decltype(m_height) {}, "(m_height) is not valid");
        assert_t::check_bool(m_channels > decltype(m_channels) {}, "(m_channels) is not valid");
        assert_t::check_bool(!std::empty(m_data), "(m_data) is not valid");
    }

    void img_png_t::shut_self()
    {
        m_data = {};
        m_channels = {};
        m_height = {};
        m_width = {};
    }

    auto img_png_t::get_width() const -> size_t
    {
        return m_width;
    }

    auto img_png_t::get_height() const -> size_t
    {
        return m_height;
    }

    auto img_png_t::get_channels() const -> size_t
    {
        return m_channels;
    }

    auto img_png_t::get_data() const -> boost::span<std::byte const>
    {
        return m_data;
    }
}
