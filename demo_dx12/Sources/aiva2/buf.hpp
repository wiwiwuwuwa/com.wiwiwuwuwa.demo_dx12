#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/buf_info.hpp>
#include <aiva2/gpu_res.hpp>

namespace aiva2
{
    struct buf_t final : public implements_t<buf_t, gpu_res_t>
    {
        // ------------------------------------------------

    public:
        buf_t(engine_t& engine, buf_info_t const& info);

        buf_t(engine_t& engine, winrt::com_ptr<ID3D12Resource> const& resource);

        ~buf_t() override;

        // ------------------------------------------------

    public:
        auto get_info() const->buf_info_t const&;

    private:
        void init_resource_from_info();

        void init_info_from_resource();

    private:
        buf_info_t m_info{};

        // ------------------------------------------------

    public:
        void set_data(boost::span<std::byte const> const& src_bin, std::optional<size_t> const& dst_pos = {}) const;

        void get_data(boost::span<std::byte> const& dst_bin, std::optional<size_t> const& src_pos = {}) const;

        // ------------------------------------------------
    };
}
