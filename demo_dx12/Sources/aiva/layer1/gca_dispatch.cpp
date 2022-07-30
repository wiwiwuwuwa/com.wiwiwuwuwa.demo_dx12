#include <pch.h>
#include <aiva/layer1/gca_dispatch.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/layer1/ro_material_compute.h>
#include <aiva/utils/asserts.h>

void aiva::layer1::GcaDispatch::Execute(aiva::layer1::Engine const& engine) const
{
	ExecuteSetPipelineState(engine);
	ExecuteSetPipelineState(engine);
	ExecuteSetComputeRootSignature(engine);
	ExecuteSetDescriptorHeaps(engine);
	ExecuteSetComputeRootDescriptorTable(engine);
	ExecuteDispatch(engine);
}

void aiva::layer1::GcaDispatch::ExecuteSetPipelineState(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& material = Material;
	aiva::utils::Asserts::CheckBool(material);

	auto const& pipelineState = material->InternalPipelineState();
	winrt::check_bool(pipelineState);

	commandList->SetPipelineState(pipelineState.get());
}

void aiva::layer1::GcaDispatch::ExecuteSetComputeRootSignature(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& material = Material;
	aiva::utils::Asserts::CheckBool(material);

	auto const& rootSignature = material->ResourceDescriptor().InternalRootSignature();
	winrt::check_bool(rootSignature);

	commandList->SetComputeRootSignature(rootSignature.get());
}

void aiva::layer1::GcaDispatch::ExecuteSetDescriptorHeaps(Engine const& engine) const
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

void aiva::layer1::GcaDispatch::ExecuteSetComputeRootDescriptorTable(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& material = Material;
	aiva::utils::Asserts::CheckBool(material);

	auto const& packedHeaps = material->ResourceDescriptor().InternalDescriptorHeaps();
	for (auto const& packedHeap : packedHeaps) winrt::check_bool(packedHeap);

	for (std::size_t i = {}; i < packedHeaps.size(); i++)
	{
		commandList->SetComputeRootDescriptorTable(i, packedHeaps[i]->GetGPUDescriptorHandleForHeapStart());
	}
}

void aiva::layer1::GcaDispatch::ExecuteDispatch(Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	commandList->Dispatch(ThreadGroupCount.x, ThreadGroupCount.y, ThreadGroupCount.z);
}
