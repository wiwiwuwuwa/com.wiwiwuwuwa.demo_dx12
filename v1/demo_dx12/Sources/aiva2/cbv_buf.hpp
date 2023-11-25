#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/gpu_eye.hpp>
#include <aiva2/cbv_buf_info.hpp>
#include <aiva2/buf_info.hpp>

namespace aiva2
{
    struct cbv_buf_t final : public implements_t<cbv_buf_t, gpu_eye_t>
    {
        // ------------------------------------------------

    public:
        using resource_type = buf_t;

        using info_type = cbv_buf_info_t;

    public:
        cbv_buf_t(engine_t& engine, std::shared_ptr<buf_t> const& resource, cbv_buf_info_t const& info = {});

        ~cbv_buf_t() override;

    public:
        template <typename t_res>
        cbv_buf_t(engine_t& engine, t_res const& resource, cbv_buf_info_t const& info = {});

        // ------------------------------------------------

    public:
        auto init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER> override;

        void bind_for_rendering(D3D12_CPU_DESCRIPTOR_HANDLE const& bind_place) const override;

        auto get_gpu_virtual_address() const->D3D12_GPU_VIRTUAL_ADDRESS;

        auto shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER> override;

        // ------------------------------------------------

    public:
        auto get_resource_ptr() const->std::shared_ptr<buf_t> const&;

        auto get_resource_ref() const->buf_t&;

    private:
        std::shared_ptr<buf_t> m_resource{};

        // ------------------------------------------------

    public:
        auto get_info() const->cbv_buf_info_t const&;

    private:
        cbv_buf_info_t m_info{};

        // ------------------------------------------------
    };
}
