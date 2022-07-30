#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct SceneActor;
}

namespace aiva::layer2
{
	struct SceneComponent : private boost::noncopyable, public std::enable_shared_from_this<SceneComponent>
	{
	private:
		friend SceneActor;

	// ----------------------------------------------------
	// Main

	private:
		SceneComponent(SceneActor const& actor);

	public:
		virtual ~SceneComponent();

	private:
		SceneActor const& mActor;
	};
}
