#pragma once
#include <pch.h>

#include <aiva/layer1/a_graphic_resource_view_fwd.h>
#include <aiva/layer1/res_view_desc_fwd.h>

namespace aiva::layer1
{
	struct ResViewDescUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		ResViewDescUtils() = delete;

	// ----------------------------------------------------
	// Utils

	public:
		static std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> GetHandle(ResViewDescType const& resViewDesc);

		static AGraphicResourceViewTypeShared GetView(ResViewDescType const& resViewDesc);

		static bool IsValid(ResViewDescType const& value);
	};
}
