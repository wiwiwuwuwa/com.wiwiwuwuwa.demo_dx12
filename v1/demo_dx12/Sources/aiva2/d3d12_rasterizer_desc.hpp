#pragma once
#include <aiva2/_core.hpp>

#include <aiva2/implements.hpp>
#include <aiva2/object.hpp>

namespace aiva2
{
    struct d3d12_rasterizer_desc_t final : public implements_t<d3d12_rasterizer_desc_t, object_t>
    {
        // ------------------------------------------------

    public:
        d3d12_rasterizer_desc_t(shader_info_t const& shader_info);

        ~d3d12_rasterizer_desc_t() override;

        // ------------------------------------------------

    public:
        auto get_data() const->D3D12_RASTERIZER_DESC;

        // ------------------------------------------------
    };
}
