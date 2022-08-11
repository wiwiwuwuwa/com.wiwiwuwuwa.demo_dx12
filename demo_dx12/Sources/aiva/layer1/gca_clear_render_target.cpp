#include <pch.h>
#include <aiva/layer1/gca_clear_render_target.h>

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

	void GcaClearRenderTarget::Execute(Engine const& engine) const
	{
		ExecuteResourceBarrier(engine);
		ExecuteClearRenderTargetView(engine);
	}

	void GcaClearRenderTarget::ExecuteResourceBarrier(Engine const& engine) const
	{
		auto const& commandList = engine.GraphicHardware().CommandList();
		winrt::check_bool(commandList);

		Asserts::CheckBool(ResViewDescUtils::IsValid(RT), "RT is not valid");
		auto const rtView = ResViewDescUtils::GetView(RT);
		Asserts::CheckBool(rtView, "RT view is not valid");

		auto const barriers = rtView->CreateDirectxBarriers(true);
		if (std::empty(barriers))
		{
			return;
		}

		commandList->ResourceBarrier(std::size(barriers), std::data(barriers));
	}

	void GcaClearRenderTarget::ExecuteClearRenderTargetView(Engine const& engine) const
	{
		auto const& commandList = engine.GraphicHardware().CommandList();
		winrt::check_bool(commandList);

		Asserts::CheckBool(ResViewDescUtils::IsValid(RT), "RT is not valid");
		auto const rtHandle = ResViewDescUtils::GetHandle(RT);
		Asserts::CheckBool(rtHandle, "RT handle is not valid");

		commandList->ClearRenderTargetView(*rtHandle, glm::value_ptr(Color), {}, {});
	}
}
