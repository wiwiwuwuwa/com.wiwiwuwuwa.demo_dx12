#include <pch.h>
#include <aiva/layer1/gca_set_scissor_rects.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>

void aiva::layer1::GcaSetScissorRects::Execute(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto rect = D3D12_RECT{};
	rect.left = Rect.x;
	rect.top = Rect.y;
	rect.right = Rect.z;
	rect.bottom = Rect.w;

	commandList->RSSetScissorRects(1, &rect);
}
