#pragma once
#include <pch.h>

#include <aiva/layer1/i_graphic_command_async.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GcaSetScissorRects final : public IGraphicCommandAsync
	{
	// ----------------------------------------------------
	// IGraphicCommandAsync

	public:
		void Execute(Engine const& engine) const override;

	// ----------------------------------------------------
	// Metadata

	public:
		glm::vec4 Rect{};
	};
}
