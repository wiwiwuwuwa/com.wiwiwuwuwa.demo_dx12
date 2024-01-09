#pragma once
#include <d3d12.h>

namespace aiva
{
    struct d3d12_resource_desc_utils_t final
    {
        // ------------------------------------------------

    private:
        d3d12_resource_desc_utils_t() = delete;

    public:
        static auto get_array_size(D3D12_RESOURCE_DESC const& desc) -> size_t;

        static auto get_subresources_count(D3D12_RESOURCE_DESC const& desc) -> size_t;

        // ------------------------------------------------
    };
} // namespace aiva
