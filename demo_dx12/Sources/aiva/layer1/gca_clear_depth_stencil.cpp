#include <pch.h>
#include <aiva/layer1/gca_clear_depth_stencil.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/i_gpu_resource_view.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/utils/asserts.h>

void aiva::layer1::GcaClearDepthStencil::Execute(Engine const& engine) const
{
	ExecuteResourceBarrier(engine);
	ExecuteClearDepthStencilView(engine);
}

void aiva::layer1::GcaClearDepthStencil::ExecuteResourceBarrier(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& heap = Heap;
	aiva::utils::Asserts::CheckBool(heap);

	auto const& view = heap->ResourceView(View);
	aiva::utils::Asserts::CheckBool(view);

	auto const& barriers = view->PrepareBarriers(true);
	if (std::empty(barriers))
	{
		return;
	}

	commandList->ResourceBarrier(std::size(barriers), std::data(barriers));
}

void aiva::layer1::GcaClearDepthStencil::ExecuteClearDepthStencilView(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& heap = Heap;
	aiva::utils::Asserts::CheckBool(heap);

	auto const& handle = heap->InternalDescriptorHandle(View);
	aiva::utils::Asserts::CheckBool(handle);

	commandList->ClearDepthStencilView(*handle, D3D12_CLEAR_FLAG_DEPTH, Depth, {}, {}, {});
}
