#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_resource_descriptor_params.h>
#include <aiva/layer1_ext/rt_resource_type.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceType TResourceType, RtResourceViewType TResourceViewType>
	void RtResourceDescriptorFunction(RtResourceDescriptorParams<TResourceType, TResourceViewType> const&);

	// ----------------------------------------------------
	// Specializations

	template <>
	void RtResourceDescriptorFunction<RtResourceType::Texture2D, RtResourceViewType::Srv>(RtResourceDescriptorParams<RtResourceType::Texture2D, RtResourceViewType::Srv> const& params);

	template <>
	void RtResourceDescriptorFunction<RtResourceType::Texture2D, RtResourceViewType::Rtv>(RtResourceDescriptorParams<RtResourceType::Texture2D, RtResourceViewType::Rtv> const& params);

	template <>
	void RtResourceDescriptorFunction<RtResourceType::Texture2D, RtResourceViewType::Dsv>(RtResourceDescriptorParams<RtResourceType::Texture2D, RtResourceViewType::Dsv> const& params);
}
