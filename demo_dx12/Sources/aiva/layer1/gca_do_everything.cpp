#include <pch.h>
#include <aiva/layer1/gca_do_everything.h>

#include <winrt/base.h>
#include <glm/gtc/type_ptr.hpp>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>

void aiva::layer1::GcaDoEveryting::Execute(aiva::layer1::Engine const& engine) const
{
	auto const& device = engine.GraphicHardware().Device();
	winrt::check_bool(device);

	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& swapChain = engine.GraphicHardware().SwapChain();
	winrt::check_bool(swapChain);

	auto const& renderTargetHeap = engine.GraphicHardware().DescriptorHeap();
	winrt::check_bool(renderTargetHeap);

	auto const& renderTargetResources = engine.GraphicHardware().RenderTargetViews();
	for (winrt::com_ptr<ID3D12Resource> const& renderTargetResource : renderTargetResources) winrt::check_bool(renderTargetResource);

	UINT const currentBackBufferIndex = swapChain->GetCurrentBackBufferIndex();
	winrt::com_ptr<ID3D12Resource> const currentBackBufferResource = renderTargetResources.at(currentBackBufferIndex);

	{
		D3D12_RESOURCE_BARRIER openBarrier{};
		openBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		openBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		openBarrier.Transition.pResource = currentBackBufferResource.get();
		openBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		openBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		openBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		commandList->ResourceBarrier(1, &openBarrier);
	}

	{
		SIZE_T const currentBackBufferHeapStart = renderTargetHeap->GetCPUDescriptorHandleForHeapStart().ptr;
		UINT const currentBackBufferHeapDelta = device->GetDescriptorHandleIncrementSize(renderTargetHeap->GetDesc().Type);

		D3D12_CPU_DESCRIPTOR_HANDLE currentBackBufferHandle{};
		currentBackBufferHandle.ptr = currentBackBufferHeapStart + static_cast<SIZE_T>(currentBackBufferIndex * currentBackBufferHeapDelta);

		commandList->ClearRenderTargetView(currentBackBufferHandle, glm::value_ptr(glm::vec4(27.0f, 32.0f, 80.0f, 255.0f) / 255.0f), 0, nullptr);
	}

	{
		D3D12_RESOURCE_BARRIER closeBarrier{};
		closeBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		closeBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		closeBarrier.Transition.pResource = currentBackBufferResource.get();
		closeBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		closeBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		closeBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

		commandList->ResourceBarrier(1, &closeBarrier);
	}
}
