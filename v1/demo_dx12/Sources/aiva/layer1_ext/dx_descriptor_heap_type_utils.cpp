#include <pch.h>
#include <aiva/layer1_ext/dx_descriptor_heap_type_utils.h>

#include <aiva/layer1_ext/rt_parse_descriptor_heap_type.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/enum_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	DxDescriptorHeapTypeUtils::DxDescriptorHeapTypeUtils()
	{
		InitParsefromResourceHeapType();
	}

	DxDescriptorHeapTypeUtils::~DxDescriptorHeapTypeUtils()
	{
		ShutParsefromResourceHeapType();
	}

	DxDescriptorHeapTypeUtils& DxDescriptorHeapTypeUtils::Instance()
	{
		static DxDescriptorHeapTypeUtils instance{};
		return instance;
	}

	D3D12_DESCRIPTOR_HEAP_TYPE DxDescriptorHeapTypeUtils::Parse(RtResourceHeapType const resourceHeapType)
	{
		auto const iter = Instance().mResourceHeapTypeToDescriptorHeapType.find(resourceHeapType);
		Asserts::CheckBool(iter != std::cend(Instance().mResourceHeapTypeToDescriptorHeapType), "Failed to parse descriptor heap type from resource heap type");

		return iter->second;
	}

	void DxDescriptorHeapTypeUtils::InitParsefromResourceHeapType()
	{
		mResourceHeapTypeToDescriptorHeapType = {};

		mResourceHeapTypeToDescriptorHeapType.insert_or_assign(RtResourceHeapType::CbvSrvUav, RtParseDescriptorHeapTypeV<RtResourceHeapType::CbvSrvUav>);
		mResourceHeapTypeToDescriptorHeapType.insert_or_assign(RtResourceHeapType::Sampler, RtParseDescriptorHeapTypeV<RtResourceHeapType::Sampler>);
		mResourceHeapTypeToDescriptorHeapType.insert_or_assign(RtResourceHeapType::Rtv, RtParseDescriptorHeapTypeV<RtResourceHeapType::Rtv>);
		mResourceHeapTypeToDescriptorHeapType.insert_or_assign(RtResourceHeapType::Dsv, RtParseDescriptorHeapTypeV<RtResourceHeapType::Dsv>);
	}

	void DxDescriptorHeapTypeUtils::ShutParsefromResourceHeapType()
	{
		mResourceHeapTypeToDescriptorHeapType = {};
	}
}
