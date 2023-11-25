#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/engine_object.hpp>
#include <aiva2/hlsl_semantic_type.hpp>

namespace aiva2
{
    struct shader_info_for_struct_t final : public implements_t<shader_info_for_struct_t, engine_object_t>
    {
        // ------------------------------------------------

    public:
        shader_info_for_struct_t(engine_t &engine, std::string_view const& text);

        ~shader_info_for_struct_t() override;

        // ------------------------------------------------

    public:
        auto get_text() const -> std::string const&;

    private:
        std::string m_text{};

        // ------------------------------------------------

    public:
        auto get_name() const -> std::string const&;

    private:
        void init_name();

        void shut_name();

    private:
        std::string m_name{};

        // ------------------------------------------------

    public:
        auto get_field_idx(std::string_view const& name) const -> std::optional<size_t>;

        auto get_field_idx(hlsl_semantic_type_t const& semantic_type, size_t const semantic_index) const -> std::optional<size_t>;

        auto get_field_idx(shader_info_for_struct_field_t const& field) const -> std::optional<size_t>;

        auto get_field_ref(size_t const index) const -> shader_info_for_struct_field_t const&;

        auto get_field_ref(std::string_view const& name) const -> shader_info_for_struct_field_t const&;

        auto get_field_ref(hlsl_semantic_type_t const& semantic_type, size_t const semantic_index) const -> shader_info_for_struct_field_t const&;

        auto get_field_ptr(size_t const index) const -> std::shared_ptr<shader_info_for_struct_field_t const>;

        auto get_field_ptr(std::string_view const& name) const -> std::shared_ptr<shader_info_for_struct_field_t const>;

        auto get_field_ptr(hlsl_semantic_type_t const& semantic_type, size_t const semantic_index) const -> std::shared_ptr<shader_info_for_struct_field_t const>;

        auto num_field() const -> size_t;

    private:
        void init_fields();

        void shut_fields();

    private:
        std::vector<std::shared_ptr<shader_info_for_struct_field_t>> m_fields{};

        // ------------------------------------------------

    public:
        auto get_input_layout_slot(size_t const field_index) const -> std::optional<size_t>;

        auto get_input_layout_slot(std::string_view const& field_name) const -> std::optional<size_t>;

        auto get_input_layout_slot(hlsl_semantic_type_t const& semantic_type, size_t const semantic_index) const -> std::optional<size_t>;

        auto get_input_layout_slot(shader_info_for_struct_field_t const& field) const -> std::optional<size_t>;

        auto get_input_layout_field_ref(size_t const slot_index) const -> shader_info_for_struct_field_t const&;

        auto get_input_layout_field_ptr(size_t const slot_index) const -> std::shared_ptr<shader_info_for_struct_field_t const>;

        auto num_input_layout_slot() const -> size_t;

    private:
        void init_input_layout_slot_indices();

        void shut_input_layout_slot_indices();

    private:
        std::vector<size_t> m_slot_index_to_field_index{};

        // ------------------------------------------------

    public:
        auto get_byte_size() const -> size_t;

    public:
        void get_byte_value(std::string_view const& field_name, buf_t const& ref_buffer, boost::span<std::byte> const& out_value) const;

        void set_byte_value(std::string_view const& field_name, boost::span<std::byte const> const& in_value, buf_t const& ref_buffer) const;

    private:
        void init_byte_layout();

        void shut_byte_layout();

    private:
        size_t m_byte_size{};

        // ------------------------------------------------
    };
}
