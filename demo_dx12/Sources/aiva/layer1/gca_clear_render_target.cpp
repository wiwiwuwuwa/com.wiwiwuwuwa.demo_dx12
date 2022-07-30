#include <pch.h>
#include <aiva/layer1/gca_clear_render_target.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/utils/asserts.h>

void aiva::layer1::GcaClearRenderTarget::Execute(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& handle = Heap ? Heap->InternalDescriptorHandle(View) : engine.GraphicHardware().ScreenRenderTargetHandle();
	aiva::utils::Asserts::CheckBool(handle);

	commandList->ClearRenderTargetView(*handle, glm::value_ptr(Color), {}, {});
}
