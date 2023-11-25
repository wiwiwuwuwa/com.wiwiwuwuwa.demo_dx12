#pragma once
#include <pch.h>

#include <aiva/layer1_ext/ro_texture_2d_fwd.h>
#include <aiva/layer1_ext/rt_resource_type.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceType>
	struct RtFindResourceClass;

	// ----------------------------------------------------
	// Utils

	template <RtResourceType TResourceType>
	using RtFindResourceClassT = typename RtFindResourceClass<TResourceType>::type;

	// ----------------------------------------------------
	// Specializations

	template <>
	struct RtFindResourceClass<RtResourceType::Texture2D>
	{
		using type = RoTexture2D;
	};
}
