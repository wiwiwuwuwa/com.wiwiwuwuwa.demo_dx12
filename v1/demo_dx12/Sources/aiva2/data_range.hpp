#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/struct.hpp>

namespace aiva2
{
    struct data_range_t final : public implements_t<data_range_t, struct_t>
    {
        // ------------------------------------------------

    public:
        auto get_offset() const -> std::size_t const&;

        void set_offset(std::size_t const& offset);

    private:
        std::size_t m_offset{};

        // ------------------------------------------------

    public:
        auto get_size() const -> std::size_t const&;

        void set_size(std::size_t const& size);

    private:
        std::size_t m_size{};

        // ------------------------------------------------
    };
}
