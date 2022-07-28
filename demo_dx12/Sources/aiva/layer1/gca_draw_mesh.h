#pragma once
#include <pch.h>

#include <aiva/layer1/e_gpu_primitive_topology.h>
#include <aiva/layer1/i_graphic_command_async.h>

namespace aiva::layer1
{
	struct Engine;
	struct RoMaterialGraphic;
}

namespace aiva::layer1
{
	struct GcaDrawMesh final : public IGraphicCommandAsync
	{
	// ----------------------------------------------------
	// IGraphicCommandAsync

	public:
		void Execute(Engine const& engine) const override;

	private:
		void ExecuteSetPipelineState(Engine const& engine) const;

		void ExecuteSetGraphicRootSignature(Engine const& engine) const;

		void ExecuteSetDescriptorHeaps(Engine const& engine) const;

		void ExecuteSetGraphicsRootDescriptorTable(Engine const& engine) const;

		void ExecuteIASetPrimitiveTopology(Engine const& engine) const;

		void ExecuteDrawIndexedInstanced(Engine const& engine) const;

	// ----------------------------------------------------
	// Resources
		
	public:
		std::shared_ptr<RoMaterialGraphic> Material{};

	// ----------------------------------------------------
	// Metadata

	public:
		EGpuPrimitiveTopology MeshTopology{};

		std::string MeshIndicesKey{};

		std::string MeshInstancesKey{};
	};
}
