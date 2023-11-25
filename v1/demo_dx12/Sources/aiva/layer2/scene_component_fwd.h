#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct SceneComponent;

	using SceneComponentType = SceneComponent;
	using SceneComponentTypeShared = std::shared_ptr<SceneComponentType>;
	using SceneComponentTypeWeak = std::weak_ptr<SceneComponentType>;
}
