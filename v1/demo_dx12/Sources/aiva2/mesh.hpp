#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/buffer_format_utils.hpp>
#include <aiva2/engine_object.hpp>

namespace aiva2
{
    struct mesh_t final : public implements_t<mesh_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        mesh_t(engine_t &engine);

        ~mesh_t() override;

        // ------------------------------------------------

    public:
        auto init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

        auto bind_for_rendering_vert() const->std::unordered_map<vert_attr_pair_t, D3D12_VERTEX_BUFFER_VIEW, boost::hash<vert_attr_pair_t>>;

        auto bind_for_rendering_indx() const->D3D12_INDEX_BUFFER_VIEW;

        auto shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

        // ------------------------------------------------

    public:
        template <typename t_element>
        void set_vert_data(vert_attr_pair_t const &pair, boost::span<t_element const> const &data);

    private:
        void set_vert_data(vert_attr_pair_t const &pair, boost::span<std::byte const> const &data, std::size_t const stride);

    private:
        std::unordered_map<vert_attr_pair_t, std::shared_ptr<vbv_buf_t>, boost::hash<vert_attr_pair_t>> m_vert_data{};

        // ------------------------------------------------

    public:
        template <typename t_element>
        void set_indx_data(boost::span<t_element const> const &data);

        auto num_indx_data() const->std::size_t;

    private:
        void set_indx_data(boost::span<std::byte const> const &data, buffer_format_t const format);

    private:
        std::shared_ptr<ibv_buf_t> m_indx_data{};

        // ------------------------------------------------
    };
}

// --------------------------------------------------------

namespace aiva2
{
    template <typename t_element>
    void mesh_t::set_vert_data(vert_attr_pair_t const& pair, boost::span<t_element const> const& data)
    {
        set_vert_data(pair, boost::as_bytes(data), sizeof(t_element));
    }

    template <typename t_element>
    void mesh_t::set_indx_data(boost::span<t_element const> const& data)
    {
        set_indx_data(boost::as_bytes(data), convert_primitive_type_to_buffer_format<t_element>());
    }
}
