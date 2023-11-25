#include <pch.h>
#include <aiva/layer1_ext/dx_dxgi_format_utils.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils_ext/e_value_type_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	DxDXGIFormatUtils::DxDXGIFormatUtils()
	{
		InitParseFromValueType();
	}

	DxDXGIFormatUtils::~DxDXGIFormatUtils()
	{
		ShutParseFromValueType();
	}

	DxDXGIFormatUtils& DxDXGIFormatUtils::Instance()
	{
		static DxDXGIFormatUtils instance{};
		return instance;
	}

	DXGI_FORMAT DxDXGIFormatUtils::ParseFromValueType(aiva::utils_ext::EValueType const valueType)
	{
		Asserts::CheckBool(EValueTypeUtils::IsValid(valueType), "Value type is not valid");

		auto const iter = Instance().mValueTypeToDXGIFormat.find(valueType);
		Asserts::CheckBool(iter != std::cend(Instance().mValueTypeToDXGIFormat), "Failed to parse dxgi format from value type");

		return iter->second;
	}

	void DxDXGIFormatUtils::InitParseFromValueType()
	{
		mValueTypeToDXGIFormat = {};

		mValueTypeToDXGIFormat.insert_or_assign(EValueType::UInt8, DXGI_FORMAT_R8_UINT);
		mValueTypeToDXGIFormat.insert_or_assign(EValueType::UInt16, DXGI_FORMAT_R16_UINT);
		mValueTypeToDXGIFormat.insert_or_assign(EValueType::UInt32, DXGI_FORMAT_R32_UINT);
		mValueTypeToDXGIFormat.insert_or_assign(EValueType::UInt32x2, DXGI_FORMAT_R32G32_UINT);
		mValueTypeToDXGIFormat.insert_or_assign(EValueType::UInt32x3, DXGI_FORMAT_R32G32B32_UINT);
		mValueTypeToDXGIFormat.insert_or_assign(EValueType::UInt32x4, DXGI_FORMAT_R32G32B32A32_UINT);
		mValueTypeToDXGIFormat.insert_or_assign(EValueType::Float32, DXGI_FORMAT_R32_FLOAT);
		mValueTypeToDXGIFormat.insert_or_assign(EValueType::Float32x2, DXGI_FORMAT_R32G32_FLOAT);
		mValueTypeToDXGIFormat.insert_or_assign(EValueType::Float32x3, DXGI_FORMAT_R32G32B32_FLOAT);
		mValueTypeToDXGIFormat.insert_or_assign(EValueType::Float32x4, DXGI_FORMAT_R32G32B32A32_FLOAT);
	}

	void DxDXGIFormatUtils::ShutParseFromValueType()
	{
		mValueTypeToDXGIFormat = {};
	}
}
