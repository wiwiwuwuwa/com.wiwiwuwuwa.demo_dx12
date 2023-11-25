#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/buffer_format.hpp>
#include <aiva2/implements.hpp>
#include <aiva2/struct.hpp>

namespace aiva2
{
    struct ibv_buf_info_t final : public implements_t<ibv_buf_info_t, struct_t>
    {
        // ------------------------------------------------

    public:
        ibv_buf_info_t() = default;

        ~ibv_buf_info_t() override = default;

        // ------------------------------------------------

    public:
        auto get_format() const->buffer_format_t;

        void set_format(buffer_format_t const format);

    private:
        buffer_format_t m_format{};

        // ------------------------------------------------
    };
}
