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
	struct GcaClearDepthStencil final : public IGraphicCommandAsync
	{
	// ----------------------------------------------------
	// IGraphicCommandAsync

	public:
		void Execute(Engine const& engine) const override;

	private:
		void ExecuteResourceBarrier(Engine const& engine) const;

		void ExecuteClearDepthStencilView(Engine const& engine) const;

	// ----------------------------------------------------
	// Resources

	public:
		std::shared_ptr<ResourceViewHeap> Heap{};

		std::string View{};

		float Depth{};
	};
}