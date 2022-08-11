#include <pch.h>
#include <aiva/layer1/gca_present.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/res_view_desc.h>
#include <aiva/layer1/res_view_desc_utils.h>
#include <aiva/utils/asserts.h>

void aiva::layer1::GcaPresent::Execute(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const rtDesc = engine.GraphicHardware().ScreenRenderTarget();
	aiva::utils::Asserts::CheckBool(ResViewDescUtils::IsValid(rtDesc), "RT desc is not valid");

	auto const rtView = ResViewDescUtils::GetView(rtDesc);
	aiva::utils::Asserts::CheckBool(rtView, "RT view is not valid");

	auto const barriers = rtView->CreateDirectxBarriers(false);
	if (std::empty(barriers))
	{
		return;
	}

	commandList->ResourceBarrier(std::size(barriers), std::data(barriers));
}
