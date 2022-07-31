#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct IShaderValue;
	struct ResourceViewHeap;
	struct RoMaterialGraphic;
	struct RoSceneGltf;
}

namespace aiva::layer1
{
	struct SceneGltfUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		SceneGltfUtils();

		~SceneGltfUtils();

	private:
		static SceneGltfUtils& Instance();

	// ----------------------------------------------------
	// Utils

	private:
		static std::vector<std::shared_ptr<RoMaterialGraphic>> LoadGraphicMaterials(RoSceneGltf const& gltf);

		static std::vector<std::shared_ptr<ResourceViewHeap>> LoadMeshMaterials(RoSceneGltf const& gltf);

		static std::vector<std::shared_ptr<IShaderValue>> LoadBufferByAccessor(RoSceneGltf const& gltf, std::size_t const accessorID);
	};
}
