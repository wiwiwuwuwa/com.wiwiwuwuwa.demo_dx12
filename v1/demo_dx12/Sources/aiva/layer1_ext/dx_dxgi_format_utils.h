#pragma once
#include <pch.h>

#include <aiva/utils_ext/e_value_type.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct DxDXGIFormatUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		DxDXGIFormatUtils();

		~DxDXGIFormatUtils();

	private:
		static DxDXGIFormatUtils& Instance();

	// ----------------------------------------------------
	// EValueType Parsing

	public:
		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::unordered_map<aiva::utils_ext::EValueType, DXGI_FORMAT>), ValueTypeToDXGIFormat);

	public:
		static DXGI_FORMAT ParseFromValueType(aiva::utils_ext::EValueType const valueType);

	private:
		void InitParseFromValueType();

		void ShutParseFromValueType();
	};
}
