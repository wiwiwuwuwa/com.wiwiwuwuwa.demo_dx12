#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_resource_heap_type.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceHeapType>
	struct RtSupportShaderAccess : public std::false_type {};

	// ----------------------------------------------------
	// Utils

	template <RtResourceHeapType TResourceHeapType>
	constexpr auto RtSupportShaderAccessV = RtSupportShaderAccess<TResourceHeapType>::value;

	// ----------------------------------------------------
	// Specializations

	template <>
	struct RtSupportShaderAccess<RtResourceHeapType::CbvSrvUav> : public std::true_type {};

	template <>
	struct RtSupportShaderAccess<RtResourceHeapType::Sampler> : public std::true_type {};
}
