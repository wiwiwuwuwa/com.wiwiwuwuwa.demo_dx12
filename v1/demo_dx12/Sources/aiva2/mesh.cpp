#include <pch.h>
#include <aiva2/mesh.hpp>

#include <aiva2/assert.hpp>
#include <aiva2/buf.hpp>
#include <aiva2/engine.hpp>
#include <aiva2/ibv_buf.hpp>
#include <aiva2/gpu_cmd_copy_resource.hpp>
#include <aiva2/graphic_executor.hpp>
#include <aiva2/vbv_buf.hpp>
#include <aiva2/vert_attr_pair.hpp>
#include <aiva2/vert_attr_pair_utils.hpp>

namespace aiva2
{
    mesh_t::mesh_t(engine_t& engine)
        : impl_type{ engine }
    {

    }

    mesh_t::~mesh_t()
    {

    }

    auto mesh_t::init_for_rendering() const -> std::vector<D3D12_RESOURCE_BARRIER>
    {
        auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

        for (auto const& [pair, vbv_buf] : m_vert_data)
        {
            assert_t::check_bool(vbv_buf, "(vbv_buf) is not valid");

            auto const vert_barriers = (*vbv_buf).init_for_rendering();
            std::copy(std::begin(vert_barriers), std::end(vert_barriers), std::back_inserter(barriers));
        }

        if (m_indx_data)
        {
            auto const indx_barriers = (*m_indx_data).init_for_rendering();
            std::copy(std::begin(indx_barriers), std::end(indx_barriers), std::back_inserter(barriers));
        }

        return barriers;
    }

    auto mesh_t::bind_for_rendering_vert() const -> std::unordered_map<vert_attr_pair_t, D3D12_VERTEX_BUFFER_VIEW, boost::hash<vert_attr_pair_t>>
    {
        auto vbv_views = std::unordered_map<vert_attr_pair_t, D3D12_VERTEX_BUFFER_VIEW, boost::hash<vert_attr_pair_t>>{};

        for (auto const& [pair, vbv_buf] : m_vert_data)
        {
            assert_t::check_bool(vbv_buf, "(vbv_buf) is not valid");

            auto const vbv_view = (*vbv_buf).bind_for_rendering();
            vbv_views.insert_or_assign(pair, vbv_view);
        }

        return vbv_views;
    }

    auto mesh_t::bind_for_rendering_indx() const -> D3D12_INDEX_BUFFER_VIEW
    {
        auto const ibv_buf = m_indx_data;
        assert_t::check_bool(ibv_buf, "(ibv_buf) is not valid");

        return (*ibv_buf).bind_for_rendering();
    }

    auto mesh_t::shut_for_rendering() const -> std::vector<D3D12_RESOURCE_BARRIER>
    {
        auto barriers = std::vector<D3D12_RESOURCE_BARRIER>{};

        for (auto const& [pair, vbv_buf] : m_vert_data)
        {
            assert_t::check_bool(vbv_buf, "(vbv_buf) is not valid");

            auto const vert_barriers = (*vbv_buf).shut_for_rendering();
            std::copy(std::begin(vert_barriers), std::end(vert_barriers), std::back_inserter(barriers));
        }

        if (m_indx_data)
        {
            auto const indx_barriers = (*m_indx_data).shut_for_rendering();
            std::copy(std::begin(indx_barriers), std::end(indx_barriers), std::back_inserter(barriers));
        }

        return barriers;
    }

    void mesh_t::set_vert_data(vert_attr_pair_t const& pair, boost::span<std::byte const> const& data, std::size_t const stride)
    {
        assert_t::check_bool(!std::empty(data), "(data) is not valid");
        assert_t::check_bool(stride > decltype(stride){}, "(stride) is not valid");

        auto src_inf = buf_info_t{};
        src_inf.set_memory(buffer_memory_t::CPU_TO_GPU);
        src_inf.set_size(data.size());

        auto src_buf = std::make_shared<buf_t>(get_engine(), src_inf);
        assert_t::check_bool(src_buf, "(src_buf) is not valid");
        (*src_buf).set_data(data);

        auto dst_inf = buf_info_t{};
        dst_inf.set_memory(buffer_memory_t::GPU_ONLY);
        dst_inf.set_size(data.size());

        auto dst_buf = std::make_shared<buf_t>(get_engine(), dst_inf);
        assert_t::check_bool(dst_buf, "(dst_buf) is not valid");

        auto cpy_cmd = std::make_shared<gpu_cmd_copy_resource_t>(get_engine());
        assert_t::check_bool(cpy_cmd, "(cpy_cmd) is not valid");

        (*cpy_cmd).set_src(src_buf);
        (*cpy_cmd).set_dst(dst_buf);

        get_engine().get_graphic_executor().execute_command(cpy_cmd);

        auto vbv_inf = vbv_buf_info_t{};
        vbv_inf.set_stride(stride);

        auto vbv_buf = std::make_shared<vbv_buf_t>(dst_buf, vbv_inf);
        assert_t::check_bool(vbv_buf, "(vbv_buf) is not valid");

        m_vert_data.emplace(pair, vbv_buf);
    }

    void mesh_t::set_indx_data(boost::span<std::byte const> const& data, buffer_format_t const format)
    {
        assert_t::check_bool(!std::empty(data), "(data) is not valid");

        auto src_inf = buf_info_t{};
        src_inf.set_memory(buffer_memory_t::CPU_TO_GPU);
        src_inf.set_size(data.size());

        auto src_buf = std::make_shared<buf_t>(get_engine(), src_inf);
        assert_t::check_bool(src_buf, "(src_buf) is not valid");
        (*src_buf).set_data(data);

        auto dst_inf = buf_info_t{};
        dst_inf.set_memory(buffer_memory_t::GPU_ONLY);
        dst_inf.set_size(data.size());

        auto dst_buf = std::make_shared<buf_t>(get_engine(), dst_inf);
        assert_t::check_bool(dst_buf, "(dst_buf) is not valid");

        auto cpy_cmd = std::make_shared<gpu_cmd_copy_resource_t>(get_engine());
        assert_t::check_bool(cpy_cmd, "(cpy_cmd) is not valid");

        (*cpy_cmd).set_src(src_buf);
        (*cpy_cmd).set_dst(dst_buf);

        get_engine().get_graphic_executor().execute_command(cpy_cmd);

        auto ibv_inf = ibv_buf_info_t{};
        ibv_inf.set_format(format);

        auto ibv_buf = std::make_shared<ibv_buf_t>(dst_buf, ibv_inf);
        assert_t::check_bool(ibv_buf, "(ibv_buf) is not valid");

        m_indx_data = ibv_buf;
    }

    auto mesh_t::num_indx_data() const -> std::size_t
    {
        assert_t::check_bool(false, "not implemented");
        return {};
    }
}
