#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct IGraphicCommandAsync
	{
		virtual ~IGraphicCommandAsync() = default;

		virtual void Execute(Engine const& engine) const = 0;
	};
}
