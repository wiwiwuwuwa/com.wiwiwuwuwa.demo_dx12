#include <pch.h>
#include <aiva/layer1/gca_draw_mesh.h>

#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/grv_srv_to_buffer.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/dict_buffer.h>

void aiva::layer1::GcaDrawMesh::Execute(Engine const& engine) const
{
	ExecuteResourceBarrier(engine);
	ExecuteSetPipelineState(engine);
	ExecuteSetGraphicRootSignature(engine);
	ExecuteSetDescriptorHeaps(engine);
	ExecuteSetGraphicsRootDescriptorTable(engine);
	ExecuteIASetPrimitiveTopology(engine);
	ExecuteDrawIndexedInstanced(engine);
}

void aiva::layer1::GcaDrawMesh::ExecuteResourceBarrier(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& material = Material;
	aiva::utils::Asserts::CheckBool(material);

	auto const& barriers = material->PrepareBarriers(true);
	if (std::empty(barriers))
	{
		return;
	}

	commandList->ResourceBarrier(std::size(barriers), std::data(barriers));
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
	if (std::empty(packedHeaps))
	{
		return;
	}

	auto unpackedHeaps = std::vector<ID3D12DescriptorHeap*>{};
	for (auto const& packedHeap : packedHeaps)
	{
		winrt::check_bool(packedHeap);

		auto const heapDesc = packedHeap->GetDesc();
		aiva::utils::Asserts::CheckBool(heapDesc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, "Heap is not shader visible");

		auto const unpackedHeap = packedHeap.get();
		aiva::utils::Asserts::CheckBool(unpackedHeap, "Unpacked heap is not valid");

		unpackedHeaps.emplace_back(unpackedHeap);
	}
	if (std::empty(unpackedHeaps))
	{
		return;
	}

	commandList->SetDescriptorHeaps(std::size(unpackedHeaps), std::data(unpackedHeaps));
}

void aiva::layer1::GcaDrawMesh::ExecuteSetGraphicsRootDescriptorTable(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& material = Material;
	aiva::utils::Asserts::CheckBool(material);

	auto const& packedHeaps = material->ResourceDescriptor().InternalDescriptorHeaps();
	if (std::empty(packedHeaps))
	{
		return;
	}

	auto unpackedHeaps = std::vector<winrt::com_ptr<ID3D12DescriptorHeap>>{};
	for (auto const& packedHeap : packedHeaps)
	{
		winrt::check_bool(packedHeap);

		auto const heapDesc = packedHeap->GetDesc();
		aiva::utils::Asserts::CheckBool(heapDesc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, "Heap is not shader visible");

		unpackedHeaps.emplace_back(packedHeap);
	}
	if (std::empty(unpackedHeaps))
	{
		return;
	}

	for (std::size_t i = {}; i < std::size(unpackedHeaps); i++)
	{
		commandList->SetGraphicsRootDescriptorTable(i, unpackedHeaps.at(i)->GetGPUDescriptorHandleForHeapStart());
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

	auto const& meshParamsHeap = resourcesDescriptor.ResourceTable().GetResourceHeap(EDescriptorHeapType::CbvSrvUav);
	aiva::utils::Asserts::CheckBool(meshParamsHeap);

	auto const& meshIndicesView = meshParamsHeap->GetView<GrvSrvToBuffer>(MeshIndicesKey);
	aiva::utils::Asserts::CheckBool(meshIndicesView);
	auto const& meshIndicesNum = meshIndicesView->Buffer().Num();

	auto const& meshInstancesView = meshParamsHeap->GetView<GrvSrvToBuffer>(MeshInstancesKey);
	auto const& meshInstancesNum = meshInstancesView ? meshInstancesView->Buffer().Num() : std::size_t{ 1 };

	commandList->DrawInstanced(meshIndicesNum, meshInstancesNum, 0, 0);
}
