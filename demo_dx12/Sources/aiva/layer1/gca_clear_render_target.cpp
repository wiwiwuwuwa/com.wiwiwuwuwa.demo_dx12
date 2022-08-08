#include <pch.h>
#include <aiva/layer1/gca_clear_render_target.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/utils/asserts.h>

void aiva::layer1::GcaClearRenderTarget::Execute(Engine const& engine) const
{
	ExecuteResourceBarrier(engine);
	ExecuteClearRenderTargetView(engine);
}

void aiva::layer1::GcaClearRenderTarget::ExecuteResourceBarrier(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& view = Heap ? Heap->GetView(View) : engine.GraphicHardware().ScreenViewObj();
	aiva::utils::Asserts::CheckBool(view);

	auto const& barriers = view->CreateDirectxBarriers(true);
	if (std::empty(barriers))
	{
		return;
	}

	commandList->ResourceBarrier(std::size(barriers), std::data(barriers));
}

void aiva::layer1::GcaClearRenderTarget::ExecuteClearRenderTargetView(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& handle = Heap ? Heap->InternalDescriptorHandle(View) : engine.GraphicHardware().ScreenViewHandle();
	aiva::utils::Asserts::CheckBool(handle);

	commandList->ClearRenderTargetView(*handle, glm::value_ptr(Color), {}, {});
}
