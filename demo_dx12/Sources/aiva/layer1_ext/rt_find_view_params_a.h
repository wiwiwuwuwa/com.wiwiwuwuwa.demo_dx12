#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_view_params.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	struct RtFindViewParamsA
	{
		using type = RtViewParams<TResourceType, TResourceViewType>;
	};

	// ----------------------------------------------------
	// Utils

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	using RtFindViewParamsAT = typename RtFindViewParamsA<TResourceType, TResourceViewType>::type;
}
