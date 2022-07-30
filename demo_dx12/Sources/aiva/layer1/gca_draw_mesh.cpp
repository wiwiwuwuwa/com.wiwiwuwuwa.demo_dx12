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
	ExecuteSetPipelineState(engine);
	ExecuteSetGraphicRootSignature(engine);
	ExecuteSetDescriptorHeaps(engine);
	ExecuteSetGraphicsRootDescriptorTable(engine);
	ExecuteIASetPrimitiveTopology(engine);
	ExecuteDrawIndexedInstanced(engine);
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
