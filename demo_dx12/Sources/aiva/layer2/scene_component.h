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

	public:
		virtual ~SceneComponent();

	private:
		SceneComponent(std::weak_ptr<aiva::layer2::SceneActor> const& actor);

	private:
		std::weak_ptr<aiva::layer2::SceneActor> mActor{};
	};
}
