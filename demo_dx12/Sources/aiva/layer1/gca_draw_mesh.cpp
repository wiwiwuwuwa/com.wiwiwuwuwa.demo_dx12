#include <pch.h>
#include <aiva/layer1/gca_draw_mesh.h>

#include <aiva/layer1/e_gpu_descriptor_heap_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/grv_srv_to_buffer.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/utils/asserts.h>

void aiva::layer1::GcaDrawMesh::Execute(Engine const& engine) const
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
		commandList->OMSetRenderTargets(1, &currentBackBufferHandle, false, nullptr);
	}

	ExecuteSetPipelineState(engine);
	ExecuteSetGraphicRootSignature(engine);
	ExecuteSetDescriptorHeaps(engine);
	ExecuteSetGraphicsRootDescriptorTable(engine);
	ExecuteIASetPrimitiveTopology(engine);
	ExecuteDrawIndexedInstanced(engine);

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

void aiva::layer1::GcaDrawMesh::ExecuteSetPipelineState(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& material = Material;
	aiva::utils::Asserts::CheckBool(material);

	auto const& pipelineState = material->InternalPipelineState();
	winrt::check_bool(pipelineState);

	commandList->SetPipelineState(pipelineState.get());
}

void aiva::layer1::GcaDrawMesh::ExecuteSetGraphicRootSignature(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& material = Material;
	aiva::utils::Asserts::CheckBool(material);

	auto const& rootSignature = material->ResourceDescriptor().InternalRootSignature();
	winrt::check_bool(rootSignature);

	commandList->SetGraphicsRootSignature(rootSignature.get());
}

void aiva::layer1::GcaDrawMesh::ExecuteSetDescriptorHeaps(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& material = Material;
	aiva::utils::Asserts::CheckBool(material);

	auto const& packedHeaps = material->ResourceDescriptor().InternalDescriptorHeaps();
	if (packedHeaps.empty()) return;

	for (auto const& packedHeap : packedHeaps) winrt::check_bool(packedHeap);

	auto& unpackedHeaps = std::vector<ID3D12DescriptorHeap*>{};
	std::transform(packedHeaps.cbegin(), packedHeaps.cend(), std::back_inserter(unpackedHeaps), [](auto const& heap) { return heap.get(); });

	commandList->SetDescriptorHeaps(unpackedHeaps.size(), unpackedHeaps.data());
}

void aiva::layer1::GcaDrawMesh::ExecuteSetGraphicsRootDescriptorTable(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& material = Material;
	aiva::utils::Asserts::CheckBool(material);

	auto const& packedHeaps = material->ResourceDescriptor().InternalDescriptorHeaps();
	for (auto const& packedHeap : packedHeaps) winrt::check_bool(packedHeap);

	for (std::size_t i = {}; i < packedHeaps.size(); i++)
	{
		commandList->SetGraphicsRootDescriptorTable(i, packedHeaps[i]->GetGPUDescriptorHandleForHeapStart());
	}
}

void aiva::layer1::GcaDrawMesh::ExecuteIASetPrimitiveTopology(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	commandList->IASetPrimitiveTopology(ToInternalEnum(MeshTopology));
}

void aiva::layer1::GcaDrawMesh::ExecuteDrawIndexedInstanced(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& material = Material;
	aiva::utils::Asserts::CheckBool(material);

	auto const& resourcesDescriptor = material->ResourceDescriptor();

	auto const& meshParamsHeap = resourcesDescriptor.ResourceTable().ResourceHeap(EGpuDescriptorHeapType::CbvSrvUav);
	aiva::utils::Asserts::CheckBool(meshParamsHeap);

	auto const& meshIndicesView = meshParamsHeap->ResourceView<GrvSrvToBuffer>(MeshIndicesKey);
	aiva::utils::Asserts::CheckBool(meshIndicesView);
	auto const& meshIndicesNum = meshIndicesView->Buffer().Num();

	auto const& meshInstancesView = meshParamsHeap->ResourceView<GrvSrvToBuffer>(MeshInstancesKey);
	auto const& meshInstancesNum = meshIndicesView ? meshIndicesView->Buffer().Num() : std::size_t{ 1 };

	commandList->DrawInstanced(meshIndicesNum, meshInstancesNum, 0, 0);
}
