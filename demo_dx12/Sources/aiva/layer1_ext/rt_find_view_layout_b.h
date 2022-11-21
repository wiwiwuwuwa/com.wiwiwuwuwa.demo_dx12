#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_find_view_layout_type_b.h>
#include <aiva/layer1_ext/rt_view_layout.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceViewType TResourceViewType>
	struct RtFindViewLayoutB
	{
		using type = RtViewLayout<RtFindViewLayoutTypeBV<TResourceViewType>>;
	};

	// ----------------------------------------------------
	// Utils

	template <RtResourceViewType TResourceViewType>
	using RtFindViewLayoutBT = typename RtFindViewLayoutB<TResourceViewType>::type;
}
