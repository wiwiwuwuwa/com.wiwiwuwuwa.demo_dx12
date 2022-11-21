#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_resource_heap_type.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct DxDescriptorHeapTypeUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		DxDescriptorHeapTypeUtils();

		~DxDescriptorHeapTypeUtils();

	private:
		static DxDescriptorHeapTypeUtils& Instance();

	// ----------------------------------------------------
	// RtResourceHeapType Parsing

	public:
		M_OBJECT_FIELD_VAR_3(private, M_SINGLE_ARG(std::unordered_map<RtResourceHeapType, D3D12_DESCRIPTOR_HEAP_TYPE>), ResourceHeapTypeToDescriptorHeapType);

	public:
		static D3D12_DESCRIPTOR_HEAP_TYPE Parse(RtResourceHeapType const resourceHeapType);

	private:
		void InitParsefromResourceHeapType();

		void ShutParsefromResourceHeapType();
	};
}
