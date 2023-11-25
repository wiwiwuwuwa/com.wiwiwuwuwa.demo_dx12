#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct RoSceneGltf;

	using RoSceneGltfType = RoSceneGltf;
	using RoSceneGltfTypeShared = std::shared_ptr<RoSceneGltfType>;
	using RoSceneGltfTypeWeak = std::weak_ptr<RoSceneGltfType>;
}
