#include <pch.h>
#include <aiva2/cbv_buf.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buf.hpp>
#include <aiva2/buf_info.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_eye_reg.hpp>
#include <aiva2/graphic_hardware.hpp>

namespace aiva2
{
    GPU_EYE_REG(cbv_buf_t);

    cbv_buf_t::cbv_buf_t(engine_t& engine, std::shared_ptr<buf_t> const& resource, cbv_buf_info_t const& info /*= {}*/)
        : impl_type{ engine }
        , m_resource{ resource }
        , m_info{ info }
    {
        
    }

    cbv_buf_t::~cbv_buf_t()
    {
        
    }

    auto cbv_buf_t::init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        return get_resource_ref().set_state(D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);
    }

    void cbv_buf_t::bind_for_rendering(D3D12_CPU_DESCRIPTOR_HANDLE const& bind_place) const
    {
        auto view_desc = D3D12_CONSTANT_BUFFER_VIEW_DESC{};
        view_desc.BufferLocation = (*get_resource_ref().get_resource()).GetGPUVirtualAddress();
        view_desc.SizeInBytes = static_cast<UINT>(get_info().get_size());

        get_engine().get_graphic_hardware().get_device().CreateConstantBufferView
        (
            /*pDesc*/ &view_desc,
            /*DestDescriptor*/ bind_place
        );
    }

    auto cbv_buf_t::get_gpu_virtual_address() const->D3D12_GPU_VIRTUAL_ADDRESS
    {
        return get_resource_ref().get_gpu_virtual_address();
    }

    auto cbv_buf_t::shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>
    {
        return get_resource_ref().set_state(D3D12_RESOURCE_STATE_COMMON);
    }

    auto cbv_buf_t::get_resource_ptr() const->std::shared_ptr<buf_t> const&
    {
        return m_resource;
    }

    auto cbv_buf_t::get_resource_ref() const->buf_t&
    {
        assert_t::check_bool(m_resource, "m_resource is not valid");
        return (*m_resource);
    }

    auto cbv_buf_t::get_info() const->cbv_buf_info_t const&
    {
        return m_info;
    }
}
