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

	public:
		using MaterialArray = std::vector<std::shared_ptr<RoMaterialGraphic>>;

		using MeshArray = std::vector<std::shared_ptr<ResourceViewHeap>>;

		using VertexArray = std::vector<std::shared_ptr<IShaderValue>>;

		using MaterialPerNodeMap = std::unordered_map<std::size_t, std::shared_ptr<RoMaterialGraphic>>;

	public:
		static MaterialPerNodeMap LoadMaterials(RoSceneGltf const& gltf);

	private:
		static MaterialArray LoadGraphicMaterials(RoSceneGltf const& gltf);

		static MeshArray LoadMeshMaterials(RoSceneGltf const& gltf);

		static VertexArray LoadBufferByAccessor(RoSceneGltf const& gltf, std::size_t const accessorID);

		static MaterialPerNodeMap LoadNodeMaterials(RoSceneGltf const& gltf, MaterialArray const& materials, MeshArray const& meshes);
	};
}
