#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct SceneActor final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		SceneActor(tiny_utf8::string const& name);

		~SceneActor();

	private:
		tiny_utf8::string mName{};
	};
}
