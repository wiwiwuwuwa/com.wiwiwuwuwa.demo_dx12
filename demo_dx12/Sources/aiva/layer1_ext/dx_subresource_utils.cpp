#include <pch.h>
#include <aiva/layer1_ext/dx_subresource_utils.h>

namespace aiva::layer1_ext
{
	std::size_t DxSubResourceUtils::ToSubResource(std::size_t const mipSlice, std::size_t const arraySlice, std::size_t const planeSlice, std::size_t const mipLevels, std::size_t const arraySize)
	{
		return mipSlice + (arraySlice * mipLevels) + (planeSlice * mipLevels * arraySize);
	}
}
