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

        // ------------------------------------------------

    public:
        auto get_data() const->D3D12_INPUT_LAYOUT_DESC const&;

    private:
        void init_data(shader_info_t const& shader_info);

        void shut_data();

    private:
        D3D12_INPUT_LAYOUT_DESC m_data{};

        // ------------------------------------------------
    };
}
