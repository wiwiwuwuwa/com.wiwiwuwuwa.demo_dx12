#include <pch.h>
#include <aiva/layer1/res_view_desc_utils.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/res_view_desc.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> ResViewDescUtils::GetHandle(ResViewDescType const& resViewDesc)
	{
		Asserts::CheckBool(IsValid(resViewDesc), "Res view desc is not valid");
		
		auto const handle = resViewDesc.Heap->InternalDescriptorHandle(resViewDesc.Name);
		Asserts::CheckBool(handle, "Handle is not valid");

		return handle;
	}

	AGraphicResourceViewTypeShared ResViewDescUtils::GetView(ResViewDescType const& resViewDesc)
	{
		Asserts::CheckBool(IsValid(resViewDesc), "Res view desc is not valid");

		auto const view = resViewDesc.Heap->GetView(resViewDesc.Name);
		Asserts::CheckBool(view, "View is not valid");

		return view;
	}

	bool ResViewDescUtils::IsValid(ResViewDescType const& value)
	{
		return value.Heap && !std::empty(value.Name);
	}
}
