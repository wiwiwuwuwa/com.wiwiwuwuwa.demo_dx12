#include <pch.h>
#include <aiva/layer1/gca_clear_depth_stencil.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/res_view_desc.h>
#include <aiva/layer1/res_view_desc_utils.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	void GcaClearDepthStencil::Execute(Engine const& engine) const
	{
		ExecuteResourceBarrier(engine);
		ExecuteClearDepthStencilView(engine);
	}

	void GcaClearDepthStencil::ExecuteResourceBarrier(Engine const& engine) const
	{
		auto const& commandList = engine.GraphicHardware().CommandList();
		winrt::check_bool(commandList);

		Asserts::CheckBool(ResViewDescUtils::IsValid(DS), "DS is not valid");
		auto const dsView = ResViewDescUtils::GetView(DS);
		Asserts::CheckBool(dsView, "DS view is not valid");

		auto const barriers = dsView->CreateDirectxBarriers(true);
		if (std::empty(barriers))
		{
			return;
		}

		commandList->ResourceBarrier(std::size(barriers), std::data(barriers));
	}

	void GcaClearDepthStencil::ExecuteClearDepthStencilView(Engine const& engine) const
	{
		auto const& commandList = engine.GraphicHardware().CommandList();
		winrt::check_bool(commandList);

		Asserts::CheckBool(ResViewDescUtils::IsValid(DS), "DS is not valid");
		auto const dsHandle = ResViewDescUtils::GetHandle(DS);
		Asserts::CheckBool(dsHandle, "DS handle is not valid");

		commandList->ClearDepthStencilView(*dsHandle, D3D12_CLEAR_FLAG_DEPTH, Depth, {}, {}, {});		
	}
}
