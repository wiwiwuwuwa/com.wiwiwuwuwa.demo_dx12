#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_resource_heap_type.h>

namespace aiva::layer1_ext
{
	// ----------------------------------------------------
	// Main

	template <RtResourceHeapType>
	struct RtParseDescriptorHeapType;

	// ----------------------------------------------------
	// Utils

	template <RtResourceHeapType TResourceHeapType>
	constexpr auto RtParseDescriptorHeapTypeV = RtParseDescriptorHeapType<TResourceHeapType>::value;

	// ----------------------------------------------------
	// Specializations

	template <>
	struct RtParseDescriptorHeapType<RtResourceHeapType::CbvSrvUav> : public std::integral_constant<D3D12_DESCRIPTOR_HEAP_TYPE, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>{};

	template <>
	struct RtParseDescriptorHeapType<RtResourceHeapType::Sampler> : public std::integral_constant<D3D12_DESCRIPTOR_HEAP_TYPE, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER> {};

	template <>
	struct RtParseDescriptorHeapType<RtResourceHeapType::Rtv> : public std::integral_constant<D3D12_DESCRIPTOR_HEAP_TYPE, D3D12_DESCRIPTOR_HEAP_TYPE_RTV> {};

	template <>
	struct RtParseDescriptorHeapType<RtResourceHeapType::Dsv> : public std::integral_constant<D3D12_DESCRIPTOR_HEAP_TYPE, D3D12_DESCRIPTOR_HEAP_TYPE_DSV> {};
}
