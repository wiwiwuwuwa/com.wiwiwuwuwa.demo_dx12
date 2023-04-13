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
        template <typename t_val>
        auto get_value_t(std::string_view const& name) const -> t_val;

        template <typename t_val>
        auto get_value_t(size_t const index) const -> t_val;

        template <typename t_val>
        auto set_value_t(std::string_view const& name, t_val const& value) -> void;

        template <typename t_val>
        auto set_value_t(size_t const index, t_val const& value) -> void;

    public:
        auto get_value(std::string_view const& name) const -> std::any;

        auto get_value(size_t const index) const -> std::any;

        auto num_value() const -> size_t;

        auto set_value(std::string_view const& name, std::any const& value) -> void;

        auto set_value(size_t const index, std::any const& value) -> void;

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
    template <typename t_val>
    auto material_property_block_t::get_value_t(std::string_view const& name) const -> t_val
    {
        auto const value = get_value(name);
        if (!value.has_value()) return {};
        if (value.type() != typeid(t_val)) return {};

        return std::any_cast<t_val>(value);
    }

    template <typename t_val>
    auto material_property_block_t::get_value_t(size_t const index) const -> t_val
    {
        auto const value = get_value(index);
        if (!value.has_value()) return {};
        if (value.type() != typeid(t_val)) return {};

        return std::any_cast<t_val>(value);
    }

    template <typename t_val>
    auto material_property_block_t::set_value_t(std::string_view const& name, t_val const& value) -> void
    {
        set_value(name, value);
    }

    template <typename t_val>
    auto material_property_block_t::set_value_t(size_t const index, t_val const& value) -> void
    {
        set_value(index, value);
    }
}
