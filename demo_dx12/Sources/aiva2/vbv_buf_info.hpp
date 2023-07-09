#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/struct.hpp>

namespace aiva2
{
    struct vbv_buf_info_t final : public implements_t<vbv_buf_info_t, struct_t>
    {
        // ------------------------------------------------

    public:
        vbv_buf_info_t() = default;

        ~vbv_buf_info_t() override = default;

        // ------------------------------------------------

    public:
        auto get_stride() const->size_t;

        void set_stride(size_t const stride);

    private:
        size_t m_stride{};

        // ------------------------------------------------
    };
}
