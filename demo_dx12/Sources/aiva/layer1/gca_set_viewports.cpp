#include <pch.h>
#include <aiva/layer1/gca_set_viewports.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>

void aiva::layer1::GcaSetViewports::Execute(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto viewport = D3D12_VIEWPORT{};
	viewport.TopLeftX = Rect.x;
	viewport.TopLeftY = Rect.y;
	viewport.Width = Rect.z - Rect.x;
	viewport.Height = Rect.w - Rect.y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	commandList->RSSetViewports(1, &viewport);
}
