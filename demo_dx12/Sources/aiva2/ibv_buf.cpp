#include <pch.h>
#include <aiva2/ibv_buf.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buf.hpp>
#include <aiva2/buffer_format_to_dxgi_format.hpp>
#include <aiva2/engine.hpp>

namespace aiva2
{
    ibv_buf_t::ibv_buf_t(std::shared_ptr<buf_t> const& resource, ibv_buf_info_t const& info)
        : impl_type{ (*resource).get_engine() }
    {
        set_resource_ptr(resource);
        set_info(info);
    }

    ibv_buf_t::~ibv_buf_t()
    {

    }

    auto ibv_buf_t::init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        return get_resource_ref().set_state(D3D12_RESOURCE_STATE_INDEX_BUFFER);
    }

    auto ibv_buf_t::bind_for_rendering() const->D3D12_INDEX_BUFFER_VIEW
    {
        auto ibv = D3D12_INDEX_BUFFER_VIEW{};
        ibv.BufferLocation = get_resource_ref().get_gpu_virtual_address();
        ibv.SizeInBytes = static_cast<decltype(ibv.SizeInBytes)>(get_resource_ref().get_info().get_size());
        ibv.Format = to_dxgi_format(get_info().get_format());

        return ibv;
    }

    auto ibv_buf_t::shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        return get_resource_ref().set_state(D3D12_RESOURCE_STATE_COMMON);
    }

    auto ibv_buf_t::get_resource_ref() const->buf_t&
    {
        assert_t::check_bool(m_resource, "(m_resource) is not valid");
        return (*m_resource);
    }

    void ibv_buf_t::set_resource_ptr(std::shared_ptr<buf_t> const& resource)
    {
        m_resource = resource;
    }

    auto ibv_buf_t::get_info() const->ibv_buf_info_t const&
    {
        return m_info;
    }

    void ibv_buf_t::set_info(ibv_buf_info_t const& info)
    {
        m_info = info;
    }
}
