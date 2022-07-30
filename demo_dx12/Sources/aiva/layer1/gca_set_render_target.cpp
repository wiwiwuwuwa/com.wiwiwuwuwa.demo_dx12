#include <pch.h>
#include <aiva/layer1/gca_set_render_target.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/resource_view_heap.h>

void aiva::layer1::GcaSetRenderTarget::Execute(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& rtHandles = RtHeap ? RtHeap->InternalDescriptorHandles() : std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>{ engine.GraphicHardware().ScreenRenderTargetHandle() };
	auto const& dsHandles = DsHeap ? DsHeap->InternalDescriptorHandles() : std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>{};

	commandList->OMSetRenderTargets(rtHandles.size(), !rtHandles.empty() ? rtHandles.data() : nullptr, false, !dsHandles.empty() ? dsHandles.data() : nullptr);
}
