#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_eye_info.hpp>

namespace aiva2
{
    struct cbv_buf_info_t final : public implements_t<cbv_buf_info_t, gpu_eye_info_t>
    {
        // ------------------------------------------------

    public:
        cbv_buf_info_t();

        ~cbv_buf_info_t() override;

        // ------------------------------------------------

    public:
        auto get_size() const->size_t;

        void set_size(size_t const size);

    private:
        size_t m_size{};

        // ------------------------------------------------
    };
}
