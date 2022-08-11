#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct SceneSystem;

	using SceneSystemType = SceneSystem;
	using SceneSystemTypeShared = std::shared_ptr<SceneSystemType>;
	using SceneSystemTypeWeak = std::weak_ptr<SceneSystemType>;
}
