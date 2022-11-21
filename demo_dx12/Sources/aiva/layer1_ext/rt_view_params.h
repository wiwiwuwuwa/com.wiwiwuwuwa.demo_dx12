#pragma once
#include <pch.h>

#include <aiva/layer1_ext/dx_subresource_utils.h>
#include <aiva/layer1_ext/rt_resource_type.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceType, RtResourceViewType>
	struct RtViewParams;

	// ----------------------------------------------------
	// Specializations

	template <>
	struct RtViewParams<RtResourceType::Texture2D, RtResourceViewType::Srv>
	{
	public:
		M_OBJECT_FIELD_VAR_3(public, std::optional<std::size_t>, MipSlice);

		M_OBJECT_FIELD_VAR_3(public, std::size_t, PlaneSlice);

	public:
		std::optional<std::size_t> ToSubResource(std::size_t const arraySize, std::size_t const mipLevels)
		{
			if (mMipSlice.has_value())
			{
				return DxSubResourceUtils::ToSubResource(mMipSlice.value(), {}, mPlaneSlice, mipLevels, arraySize);
			}
			else
			{
				return {};
			}
		}
	};

	template <>
	struct RtViewParams<RtResourceType::Texture2D, RtResourceViewType::Rtv>
	{
	public:
		M_OBJECT_FIELD_VAR_3(public, std::size_t, MipSlice);

		M_OBJECT_FIELD_VAR_3(public, std::size_t, PlaneSlice);

	public:
		std::size_t ToSubResource(std::size_t const arraySize, std::size_t const mipLevels)
		{
			return DxSubResourceUtils::ToSubResource(mMipSlice, {}, mPlaneSlice, mipLevels, arraySize);
		}
	};

	template <>
	struct RtViewParams<RtResourceType::Texture2D, RtResourceViewType::Dsv>
	{
	public:
		M_OBJECT_FIELD_VAR_3(public, std::size_t, MipSlice);

	public:
		std::size_t ToSubResource(std::size_t const arraySize, std::size_t const mipLevels)
		{
			return DxSubResourceUtils::ToSubResource(mMipSlice, {}, {}, mipLevels, arraySize);
		}
	};
}
