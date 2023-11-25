#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_resource_type.h>
#include <aiva/layer1_ext/rt_resource_view_fwd.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>
#include <aiva/layer1_ext/ut_barriers_flags.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	D3D12_RESOURCE_STATES RtResourceStateFunction(RtResourceView<TResourceType, TResourceViewType> const&, UtBarriersFlags const);

	// ----------------------------------------------------
	// Specializations

	template <>
	D3D12_RESOURCE_STATES RtResourceStateFunction<RtResourceType::Texture2D, RtResourceViewType::Srv>(RtResourceView<RtResourceType::Texture2D, RtResourceViewType::Srv> const&, UtBarriersFlags const);

	template <>
	D3D12_RESOURCE_STATES RtResourceStateFunction<RtResourceType::Texture2D, RtResourceViewType::Rtv>(RtResourceView<RtResourceType::Texture2D, RtResourceViewType::Rtv> const&, UtBarriersFlags const);
}
