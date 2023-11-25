#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/object.hpp>

namespace aiva2
{
    struct d3d12_input_layout_desc_t final : public implements_t<d3d12_input_layout_desc_t, object_t>
    {
        // ------------------------------------------------

    public:
        d3d12_input_layout_desc_t(shader_info_t const& shader_info);

        ~d3d12_input_layout_desc_t() override;

    public:
        auto get_data() const -> D3D12_INPUT_LAYOUT_DESC const&;

        // ------------------------------------------------

    private:
        void init_element_descs(shader_info_t const& shader_info);

        void shut_element_descs();

    private:
        std::vector<D3D12_INPUT_ELEMENT_DESC> m_element_descs{};

        // ------------------------------------------------

    private:
        void init_layout_desc(shader_info_t const& shader_info);

        void shut_layout_desc();

    private:
        D3D12_INPUT_LAYOUT_DESC m_layout_desc{};

        // ------------------------------------------------
    };
}
