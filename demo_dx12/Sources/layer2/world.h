#pragma once
#include <pch.h>

#include <memory>
#include <boost/core/noncopyable.hpp>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer2
{
	struct World final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		World();

		~World();

		void Run();

	private:
		std::unique_ptr<aiva::layer1::Engine> mEngine{};
	};
}