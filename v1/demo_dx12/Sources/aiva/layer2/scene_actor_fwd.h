#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct SceneActor;

	using SceneActorType = SceneActor;
	using SceneActorTypeShared = std::shared_ptr<SceneActorType>;
	using SceneActorTypeWeak = std::weak_ptr<SceneActorType>;
}
