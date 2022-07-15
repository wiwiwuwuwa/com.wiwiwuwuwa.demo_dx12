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
		void Execute(aiva::layer1::Engine const& engine) const override;

	public:
		glm::uvec3 ThreadGroupCount{};

		std::shared_ptr<aiva::layer1::RoMaterialCompute> ComputeMaterial{};
	};
}
