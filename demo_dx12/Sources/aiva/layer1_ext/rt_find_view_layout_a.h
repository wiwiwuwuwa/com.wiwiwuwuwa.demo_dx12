#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_find_view_layout_type_a.h>
#include <aiva/layer1_ext/rt_view_layout.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceHeapType TResourceHeapType>
	struct RtFindViewLayoutA
	{
		using type = RtViewLayout<RtFindViewLayoutTypeAV<TResourceHeapType>>;
	};

	// ----------------------------------------------------
	// Utils

	template <RtResourceHeapType TResourceHeapType>
	using RtFindViewLayoutAT = typename RtFindViewLayoutA<TResourceHeapType>::type;
}
