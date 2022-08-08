#include <pch.h>
#include <aiva/layer1/gca_present.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/utils/asserts.h>

void aiva::layer1::GcaPresent::Execute(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& viewObj = engine.GraphicHardware().ScreenViewObj();
	aiva::utils::Asserts::CheckBool(viewObj);

	auto const& barriers = viewObj->CreateDirectxBarriers(false);
	if (std::empty(barriers))
	{
		return;
	}

	commandList->ResourceBarrier(std::size(barriers), std::data(barriers));
}
