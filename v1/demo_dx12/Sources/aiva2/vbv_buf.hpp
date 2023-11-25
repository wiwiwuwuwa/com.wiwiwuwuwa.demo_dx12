#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/vbv_buf_info.hpp>

namespace aiva2
{
    struct vbv_buf_t : public implements_t<vbv_buf_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        vbv_buf_t(std::shared_ptr<buf_t> const& resource, vbv_buf_info_t const& info);

        ~vbv_buf_t() override;

        // ------------------------------------------------

    public:
        auto init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

        auto bind_for_rendering() const->D3D12_VERTEX_BUFFER_VIEW;

        auto shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

        // ------------------------------------------------

    private:
        auto get_resource_ref() const->buf_t&;

        void set_resource_ptr(std::shared_ptr<buf_t> const& resource);

    private:
        std::shared_ptr<buf_t> m_resource{};

        // ------------------------------------------------

    private:
        auto get_info() const->vbv_buf_info_t const&;

        void set_info(vbv_buf_info_t const& info);

    private:
        vbv_buf_info_t m_info{};

        // ------------------------------------------------
    };
}
