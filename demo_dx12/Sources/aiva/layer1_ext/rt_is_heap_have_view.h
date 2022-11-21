#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_find_resource_heap_type.h>
#include <aiva/layer1_ext/rt_resource_heap_type.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceHeapType TResourceHeapType, RtResourceViewType TResourceViewType>
	struct RtIsHeapHaveView : public std::bool_constant<TResourceHeapType == RtFindResourceHeapTypeV<TResourceViewType>> {};

	// ----------------------------------------------------
	// Utils

	template <RtResourceHeapType TResourceHeapType, RtResourceViewType TResourceViewType>
	constexpr auto RtIsHeapHaveViewV = RtIsHeapHaveView<TResourceHeapType, TResourceViewType>::value;
}
