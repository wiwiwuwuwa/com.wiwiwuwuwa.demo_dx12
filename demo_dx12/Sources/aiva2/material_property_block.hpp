#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>

namespace aiva2
{
    struct material_property_block_t final : public implements_t<material_property_block_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        material_property_block_t(std::shared_ptr<shader_info_for_struct_t> const& shader_info);

        ~material_property_block_t() override;

        // ------------------------------------------------

    public:
        auto get_shader_info_ptr() const -> std::shared_ptr<shader_info_for_struct_t const> const&;

        auto get_shader_info_ref() const -> shader_info_for_struct_t const&;

    private:
        std::shared_ptr<shader_info_for_struct_t const> m_shader_info{};

        // ------------------------------------------------

    public:
        void get_byte_value(std::string_view const& field_name, boost::span<std::byte> const& out_value) const;

        void set_byte_value(std::string_view const& field_name, boost::span<std::byte const> const& in_value) const;

        // ------------------------------------------------

    private:
        auto get_value_buffer_ptr() const -> std::shared_ptr<cbv_buf_t const> const&;

        auto get_value_buffer_ref() const -> cbv_buf_t const&;

    private:
        void init_values_buffer();

        void shut_values_buffer();

    private:
        std::shared_ptr<cbv_buf_t const> m_values_buffer{};

        // ------------------------------------------------

    public:
        auto init_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

        auto get_gpu_virtual_address() const->D3D12_GPU_VIRTUAL_ADDRESS;

        auto shut_for_rendering() const->std::vector<D3D12_RESOURCE_BARRIER>;

        // ------------------------------------------------
    };
}

// --------------------------------------------------------

#include <aiva2/assert.hpp>

namespace aiva2
{

}
