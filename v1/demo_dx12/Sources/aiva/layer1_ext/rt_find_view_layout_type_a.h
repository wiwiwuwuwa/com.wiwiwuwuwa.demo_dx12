#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_resource_heap_type.h>
#include <aiva/layer1_ext/rt_view_layout_type.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceHeapType>
	struct RtFindViewLayoutTypeA;

	// ----------------------------------------------------
	// Utils

	template <RtResourceHeapType TResourceHeapType>
	constexpr auto RtFindViewLayoutTypeAV = RtFindViewLayoutTypeA<TResourceHeapType>::value;

	// ----------------------------------------------------
	// Specializations

	template <>
	struct RtFindViewLayoutTypeA<RtResourceHeapType::CbvSrvUav> : public std::integral_constant<RtViewLayoutType, RtViewLayoutType::GPU> {};

	template <>
	struct RtFindViewLayoutTypeA<RtResourceHeapType::Sampler> : public std::integral_constant<RtViewLayoutType, RtViewLayoutType::GPU> {};

	template <>
	struct RtFindViewLayoutTypeA<RtResourceHeapType::Rtv> : public std::integral_constant<RtViewLayoutType, RtViewLayoutType::CPU> {};

	template <>
	struct RtFindViewLayoutTypeA<RtResourceHeapType::Dsv> : public std::integral_constant<RtViewLayoutType, RtViewLayoutType::CPU> {};
}
