#include <pch.h>
#include <aiva/layer1/gca_set_render_target.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/utils/asserts.h>

void aiva::layer1::GcaSetRenderTarget::Execute(Engine const& engine) const
{
	ExecuteResourceBarrier(engine);
	ExecuteSetRenderTargets(engine);
}

void aiva::layer1::GcaSetRenderTarget::ExecuteResourceBarrier(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto allBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

	auto const& rtBarriers = RtHeap ? RtHeap->PrepareBarriers(true) : engine.GraphicHardware().ScreenViewObj()->CreateDirectxBarriers(true);
	std::copy(std::cbegin(rtBarriers), std::cend(rtBarriers), std::back_inserter(allBarriers));

	auto const& dsBarriers = DsHeap ? DsHeap->PrepareBarriers(true) : decltype(allBarriers){};
	std::copy(std::cbegin(dsBarriers), std::cend(dsBarriers), std::back_inserter(allBarriers));

	if (std::empty(allBarriers))
	{
		return;
	}

	commandList->ResourceBarrier(std::size(allBarriers), std::data(allBarriers));
}

void aiva::layer1::GcaSetRenderTarget::ExecuteSetRenderTargets(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& rtHandles = RtHeap ? RtHeap->InternalDescriptorHandles() : std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>{ engine.GraphicHardware().ScreenViewHandle() };
	auto const& dsHandles = DsHeap ? DsHeap->InternalDescriptorHandles() : std::vector<D3D12_CPU_DESCRIPTOR_HANDLE>{};

	commandList->OMSetRenderTargets(rtHandles.size(), !rtHandles.empty() ? rtHandles.data() : nullptr, false, !dsHandles.empty() ? dsHandles.data() : nullptr);
}
