#pragma once
#include <pch.h>

#include <aiva/layer1/i_graphic_command_async.h>
#include <aiva/layer1/res_view_desc.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GcaClearRenderTarget final : public IGraphicCommandAsync
	{
	// ----------------------------------------------------
	// IGraphicCommandAsync

	public:
		void Execute(Engine const& engine) const override;

	private:
		void ExecuteResourceBarrier(Engine const& engine) const;

		void ExecuteClearRenderTargetView(Engine const& engine) const;

	// ----------------------------------------------------
	// Resources

	public:
		ResViewDesc RT{};

		glm::vec4 Color{};
	};
}
