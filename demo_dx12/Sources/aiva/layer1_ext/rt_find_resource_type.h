#pragma once
#include <pch.h>

#include <aiva/layer1_ext/ro_texture_2d_fwd.h>
#include <aiva/layer1_ext/rt_resource_type.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <typename>
	struct RtFindResourceType;

	// ----------------------------------------------------
	// Utils

	template <typename TObjectType>
	constexpr auto RtFindResourceTypeV = RtFindResourceType<TObjectType>::value;

	// ----------------------------------------------------
	// Specializations

	template <>
	struct RtFindResourceType<RoTexture2D> : public std::integral_constant<RtResourceType, RtResourceType::Texture2D> {};
}
