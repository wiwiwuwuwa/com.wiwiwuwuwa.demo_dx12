#include <aiva/d3d12_resource_desc_utils.hpp>

namespace aiva
{
    // ------------------------------------------------

    auto d3d12_resource_desc_utils_t::get_array_size(D3D12_RESOURCE_DESC const& desc) -> size_t
    {
        if (desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D)
        {
            return size_t{ 1 };
        }

        return static_cast<size_t>(desc.DepthOrArraySize);
    }

    auto d3d12_resource_desc_utils_t::get_subresources_count(D3D12_RESOURCE_DESC const& desc) -> size_t
    {
        return static_cast<size_t>(desc.MipLevels) * get_array_size(desc);
    }

    // ------------------------------------------------
} // namespace aiva
