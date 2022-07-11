#pragma once
#include <pch.h>

#include <aiva/layer1/i_graphic_command_async.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GcaDoEveryting final : public aiva::layer1::IGraphicCommandAsync
	{
	public:
		void Execute(aiva::layer1::Engine const& engine) const override;
	};
}
