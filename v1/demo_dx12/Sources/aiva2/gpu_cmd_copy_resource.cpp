#include <pch.h>
#include <aiva2/gpu_cmd_copy_resource.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/gpu_res.hpp>
#include <aiva2/graphic_hardware.hpp>

namespace aiva2
{
    gpu_cmd_copy_resource_t::gpu_cmd_copy_resource_t(engine_t& engine)
        : impl_type{ engine }
    {

    }

    gpu_cmd_copy_resource_t::~gpu_cmd_copy_resource_t()
    {

    }
    
    void gpu_cmd_copy_resource_t::execute()
    {
        execute_resource_barrier();
        execute_copy_resource();
    }

    void gpu_cmd_copy_resource_t::execute_resource_barrier()
    {
        assert_t::check_bool(m_src, "(m_src) is not valid");
        assert_t::check_bool(m_dst, "(m_dst) is not valid");

        for (auto const& resource_barrier : (*m_src).set_state(D3D12_RESOURCE_STATE_COPY_SOURCE))
        {
            get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
        }

        for (auto const& resource_barrier : (*m_dst).set_state(D3D12_RESOURCE_STATE_COPY_DEST))
        {
            get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
        }
    }

    void gpu_cmd_copy_resource_t::execute_copy_resource()
    {
        assert_t::check_bool(m_src, "(m_src) is not valid");
        assert_t::check_bool(m_dst, "(m_dst) is not valid");

        auto const raw_src = (*m_src).get_resource().get();
        auto const raw_dst = (*m_dst).get_resource().get();

        assert_t::check_bool(raw_src, "(raw_src) is not valid");
        assert_t::check_bool(raw_dst, "(raw_dst) is not valid");

        get_engine().get_graphic_hardware().get_command_list().CopyResource
        (
            /*dst*/ raw_dst,
            /*src*/ raw_src
        );
    }

    auto gpu_cmd_copy_resource_t::get_src() const->std::shared_ptr<gpu_res_t> const&
    {
        return m_src;
    }

    void gpu_cmd_copy_resource_t::set_src(std::shared_ptr<gpu_res_t> const& src)
    {
        m_src = src;
    }

    auto gpu_cmd_copy_resource_t::get_dst() const->std::shared_ptr<gpu_res_t> const&
    {
        return m_dst;
    }

    void gpu_cmd_copy_resource_t::set_dst(std::shared_ptr<gpu_res_t> const& dst)
    {
        m_dst = dst;
    }
}
