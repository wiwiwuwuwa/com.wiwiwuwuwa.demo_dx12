#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_find_resource_heap_type.h>
#include <aiva/layer1_ext/rt_find_view_layout_type_a.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceViewType TResourceViewType>
	struct RtFindViewLayoutTypeB : public std::integral_constant<RtViewLayoutType, RtFindViewLayoutTypeAV<RtFindResourceHeapTypeV<TResourceViewType>>> {};

	// ----------------------------------------------------
	// Utils

	template <RtResourceViewType TResourceViewType>
	constexpr auto RtFindViewLayoutTypeBV = RtFindViewLayoutTypeB<TResourceViewType>::value;
}
