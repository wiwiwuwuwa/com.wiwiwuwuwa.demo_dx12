#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_resource_heap_type.h>
#include <aiva/layer1_ext/rt_resource_view_type.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceViewType>
	struct RtFindResourceHeapType;

	// ----------------------------------------------------
	// Utils

	template <RtResourceViewType TResourceViewType>
	constexpr auto RtFindResourceHeapTypeV = RtFindResourceHeapType<TResourceViewType>::value;

	// ----------------------------------------------------
	// Specializations

	template <>
	struct RtFindResourceHeapType<RtResourceViewType::Cbv> : public std::integral_constant<RtResourceHeapType, RtResourceHeapType::CbvSrvUav> {};

	template <>
	struct RtFindResourceHeapType<RtResourceViewType::Srv> : public std::integral_constant<RtResourceHeapType, RtResourceHeapType::CbvSrvUav> {};

	template <>
	struct RtFindResourceHeapType<RtResourceViewType::Uav> : public std::integral_constant<RtResourceHeapType, RtResourceHeapType::CbvSrvUav> {};

	template <>
	struct RtFindResourceHeapType<RtResourceViewType::Sampler> : public std::integral_constant<RtResourceHeapType, RtResourceHeapType::Sampler> {};

	template <>
	struct RtFindResourceHeapType<RtResourceViewType::Rtv> : public std::integral_constant<RtResourceHeapType, RtResourceHeapType::Rtv> {};

	template <>
	struct RtFindResourceHeapType<RtResourceViewType::Dsv> : public std::integral_constant<RtResourceHeapType, RtResourceHeapType::Dsv> {};
}
