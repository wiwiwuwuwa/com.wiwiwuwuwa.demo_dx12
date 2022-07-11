#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct IGraphicCommand
	{
	public:
		virtual ~IGraphicCommand() = default;
		virtual void Execute(aiva::layer1::Engine const& engine) const = 0;
	};
}
