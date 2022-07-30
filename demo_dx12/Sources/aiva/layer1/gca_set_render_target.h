#pragma once
#include <pch.h>

#include <aiva/layer1/i_graphic_command_async.h>

namespace aiva::layer1
{
	struct Engine;
	struct ResourceViewHeap;
}

namespace aiva::layer1
{
	struct GcaSetRenderTarget final : public IGraphicCommandAsync
	{
	// ----------------------------------------------------
	// IGraphicCommandAsync

	public:
		void Execute(Engine const& engine) const override;

	// ----------------------------------------------------
	// Resources

	public:
		std::shared_ptr<ResourceViewHeap> RtHeap{};

		std::shared_ptr<ResourceViewHeap> DsHeap{};
	};
}
