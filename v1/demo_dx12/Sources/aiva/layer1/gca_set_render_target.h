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
	struct GcaSetRenderTarget final : public IGraphicCommandAsync
	{
	// ----------------------------------------------------
	// IGraphicCommandAsync

	public:
		void Execute(Engine const& engine) const override;

	private:
		void ExecuteResourceBarrier(Engine const& engine) const;

		void ExecuteSetRenderTargets(Engine const& engine) const;

	// ----------------------------------------------------
	// Resources

	public:
		std::vector<ResViewDescType> RTs{};

		std::vector<ResViewDescType> DSs{};
	};
}
