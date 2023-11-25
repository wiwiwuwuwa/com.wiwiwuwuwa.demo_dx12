#pragma once
#include <pch.h>

#include <aiva/layer1/i_graphic_command_async.h>

namespace aiva::layer1
{
	struct Engine;
	struct RoMaterialCompute;
}

namespace aiva::layer1
{
	struct GcaDispatch final : public aiva::layer1::IGraphicCommandAsync
	{
	public:
		void Execute(Engine const& engine) const override;

	private:
		void ExecuteSetPipelineState(Engine const& engine) const;

		void ExecuteSetComputeRootSignature(Engine const& engine) const;

		void ExecuteSetDescriptorHeaps(Engine const& engine) const;

		void ExecuteSetComputeRootDescriptorTable(Engine const& engine) const;

		void ExecuteResourceBarrier(Engine const& engine) const;

		void ExecuteDispatch(Engine const& engine) const;

	public:
		glm::uvec3 ThreadGroupCount{};

		std::shared_ptr<RoMaterialCompute> Material{};
	};
}
