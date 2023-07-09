#include <pch.h>
#include <aiva2/vbv_buf.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buf.hpp>
#include <aiva2/engine.hpp>

namespace aiva2
{
    vbv_buf_t::vbv_buf_t(std::shared_ptr<buf_t> const& resource, vbv_buf_info_t const& info)
        : impl_type{ (*resource).get_engine() }
    {
        set_resource_ptr(resource);
        set_info(info);
    }

    vbv_buf_t::~vbv_buf_t()
    {

    }

    auto vbv_buf_t::init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        return get_resource_ref().set_state(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
    }

    auto vbv_buf_t::bind_for_rendering() const->D3D12_VERTEX_BUFFER_VIEW
    {
        auto vbv = D3D12_VERTEX_BUFFER_VIEW{};
        vbv.BufferLocation = get_resource_ref().get_gpu_virtual_address();
        vbv.SizeInBytes = static_cast<decltype(vbv.SizeInBytes)>(get_resource_ref().get_info().get_size());
        vbv.StrideInBytes = static_cast<decltype(vbv.StrideInBytes)>(get_info().get_stride());

        return vbv;
    }

    auto vbv_buf_t::shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        return get_resource_ref().set_state(D3D12_RESOURCE_STATE_COMMON);
    }

    auto vbv_buf_t::get_resource_ref() const->buf_t&
    {
        assert_t::check_bool(m_resource, "(m_resource) is not valid");
        return (*m_resource);
    }

    void vbv_buf_t::set_resource_ptr(std::shared_ptr<buf_t> const& resource)
    {
        m_resource = resource;
    }

    auto vbv_buf_t::get_info() const->vbv_buf_info_t const&
    {
        return m_info;
    }

    void vbv_buf_t::set_info(vbv_buf_info_t const& info)
    {
        m_info = info;
    }
}
