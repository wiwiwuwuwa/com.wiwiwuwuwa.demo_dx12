#include <pch.h>
#include <aiva2/gpu_cmd_copy_buf_to_tex_2d.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buf.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/graphic_hardware.hpp>
#include <aiva2/math.hpp>
#include <aiva2/tex_2d.hpp>

namespace aiva2
{
    gpu_cmd_copy_buf_to_tex_2d_t::gpu_cmd_copy_buf_to_tex_2d_t(engine_t& engine)
        : impl_type{ engine }
    {

    }

    gpu_cmd_copy_buf_to_tex_2d_t::~gpu_cmd_copy_buf_to_tex_2d_t()
    {

    }

    void gpu_cmd_copy_buf_to_tex_2d_t::execute()
    {
        execute_resource_barrier();
        execute_copy_texture_region();
    }

    void gpu_cmd_copy_buf_to_tex_2d_t::execute_resource_barrier()
    {
        assert_t::check_bool(m_src, "(m_src) is not valid");
        assert_t::check_bool(m_dst, "(m_dst) is not valid");

        for (auto const& resource_barrier : (*m_src).set_state(D3D12_RESOURCE_STATE_COPY_SOURCE))
        {
            get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
        }

        for (auto const& resource_barrier : (*m_dst).set_state(D3D12_RESOURCE_STATE_COPY_DEST, 0))
        {
            get_engine().get_graphic_hardware().get_command_list().ResourceBarrier(1, &resource_barrier);
        }
    }

    void gpu_cmd_copy_buf_to_tex_2d_t::execute_copy_texture_region()
    {
        assert_t::check_bool(m_src, "(m_src) is not valid");
        assert_t::check_bool(m_dst, "(m_dst) is not valid");

        auto const raw_src = (*m_src).get_resource().get();
        auto const raw_dst = (*m_dst).get_resource().get();

        assert_t::check_bool(raw_src, "(raw_src) is not valid");
        assert_t::check_bool(raw_dst, "(raw_dst) is not valid");

        auto const src_dsc = (*raw_src).GetDesc();
        auto const dst_dsc = (*raw_dst).GetDesc();

        auto src_loc = D3D12_TEXTURE_COPY_LOCATION{};
        src_loc.pResource = raw_src;
        src_loc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        src_loc.PlacedFootprint.Offset = {};
        src_loc.PlacedFootprint.Footprint.Format = dst_dsc.Format;
        src_loc.PlacedFootprint.Footprint.Width = static_cast<UINT>(dst_dsc.Width);
        src_loc.PlacedFootprint.Footprint.Height = dst_dsc.Height;
        src_loc.PlacedFootprint.Footprint.Depth = static_cast<UINT>(dst_dsc.DepthOrArraySize);
        src_loc.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(align_up(static_cast<size_t>(src_dsc.Width / dst_dsc.Height), D3D12_TEXTURE_DATA_PITCH_ALIGNMENT));

        auto dst_loc = D3D12_TEXTURE_COPY_LOCATION{};
        dst_loc.pResource = raw_dst;
        dst_loc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        dst_loc.SubresourceIndex = {};

        get_engine().get_graphic_hardware().get_command_list().CopyTextureRegion
        (
            /*dst*/ &dst_loc,
            /*dstX*/ {},
            /*dstY*/ {},
            /*dstZ*/ {},
            /*src*/ &src_loc,
            /*srcBox*/ {}
        );
    }

    auto gpu_cmd_copy_buf_to_tex_2d_t::get_src() const->std::shared_ptr<buf_t> const&
    {
        return m_src;
    }

    void gpu_cmd_copy_buf_to_tex_2d_t::set_src(std::shared_ptr<buf_t> const& src)
    {
        m_src = src;
    }

    auto gpu_cmd_copy_buf_to_tex_2d_t::get_dst() const->std::shared_ptr<tex_2d_t> const&
    {
        return m_dst;
    }

    void gpu_cmd_copy_buf_to_tex_2d_t::set_dst(std::shared_ptr<tex_2d_t> const& dst)
    {
        m_dst = dst;
    }
}
