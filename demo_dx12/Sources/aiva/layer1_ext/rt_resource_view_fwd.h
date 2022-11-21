#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_resource_type.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>

namespace aiva::layer1_ext
{
	template <RtResourceType, RtResourceViewType>
	struct RtResourceView;
}
