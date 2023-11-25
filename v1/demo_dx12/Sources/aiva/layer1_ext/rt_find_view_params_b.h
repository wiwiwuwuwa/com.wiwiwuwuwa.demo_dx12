#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_find_resource_type.h>
#include <aiva/layer1_ext/rt_find_view_params_a.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <typename TResourceClass, RtResourceViewType TResourceViewType>
	struct RtFindViewParamsB
	{
		using type = RtViewParams<RtFindResourceTypeV<TResourceClass>, TResourceViewType>;
	};

	// ----------------------------------------------------
	// Utils

	template <typename TResourceClass, RtResourceViewType TResourceViewType>
	using RtFindViewParamsBT = typename RtFindViewParamsB<TResourceClass, TResourceViewType>::type;
}
