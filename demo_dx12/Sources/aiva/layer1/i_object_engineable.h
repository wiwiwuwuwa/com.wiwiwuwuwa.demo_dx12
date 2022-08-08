#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct IObjectEngineable
	{
	// ----------------------------------------------------
	// Main

	public:
		using EngineType = Engine;

	protected:
		IObjectEngineable(EngineType const& engine);

	public:
		virtual ~IObjectEngineable();

	// ----------------------------------------------------
	// Engine

	public:
		EngineType const& Engine() const;

	private:
		EngineType const& mEngine;
	};
}
