#include <pch.h>
#include <aiva/layer1/gca_dispatch.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/layer1/ro_material_compute.h>
#include <aiva/utils/asserts.h>

void aiva::layer1::GcaDispatch::Execute(aiva::layer1::Engine const& engine) const
{
	auto const& commandList = engine.GraphicHardware().CommandList();
	winrt::check_bool(commandList);

	auto const& computeMaterial = ComputeMaterial;
	aiva::utils::Asserts::CheckBool(computeMaterial);

	auto const& pipelineState = computeMaterial->InternalPipelineState();
	winrt::check_bool(pipelineState);

	commandList->SetPipelineState(pipelineState.get());

	auto const& resourceDescriptor = computeMaterial->ResourceDescriptor();

	auto const& rootSignature = resourceDescriptor.InternalRootSignature();
	winrt::check_bool(rootSignature);

	commandList->SetComputeRootSignature(rootSignature.get());

	commandList->Dispatch(ThreadGroupCount.x, ThreadGroupCount.y, ThreadGroupCount.z);
}
