#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct GrvSrvToBuffer;
	struct ResourceViewHeap;
	struct RoMaterialGraphic;
	struct RoSceneGltf;
}

namespace aiva::utils
{
	struct DictStruct;
	struct IBoxedValue;
	struct MetaField;
	struct MetaStruct;
}

namespace aiva::layer1
{
	struct SceneGltfUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		SceneGltfUtils() = delete;

	// ----------------------------------------------------
	// Aliases

	public: // TODO: Yes, I know, it was a bad idea
		using BoxedValueElementType = aiva::utils::IBoxedValue;

		using BoxedValuePointerType = std::shared_ptr<BoxedValueElementType>;

		using BoxedValuePointerArrayType = std::vector<BoxedValuePointerType>;

		using DictStructElementType = aiva::utils::DictStruct;

		using DictStructPointerType = std::shared_ptr<DictStructElementType>;

		using MetaFieldElementType = aiva::utils::MetaField;

		using MetaFieldPointerType = std::shared_ptr<MetaFieldElementType>;

		using MetaStructElementType = aiva::utils::MetaStruct;

		using MetaStructPointerType = std::shared_ptr<MetaStructElementType>;

		using ResourceViewHeapElementType = ResourceViewHeap;

		using ResourceViewHeapPointerType = std::shared_ptr<ResourceViewHeapElementType>;

		using ResourceViewHeapPointerArrayType = std::vector<ResourceViewHeapPointerType>;

		using MaterialGraphicElementType = RoMaterialGraphic;

		using MaterialGraphicPointerType = std::shared_ptr<RoMaterialGraphic>;

		using MaterialGraphicPointerArrayType = std::vector<MaterialGraphicPointerType>;

		using ModelDict = std::unordered_map<std::size_t, MaterialGraphicPointerType>;

		using SceneElementType = RoSceneGltf;

		using ScenePointerType = std::shared_ptr<SceneElementType>;

		using SrvToBufferElementType = GrvSrvToBuffer;

		using SrvToBufferPointerType = std::shared_ptr<SrvToBufferElementType>;

	// ----------------------------------------------------
	// Models

	public:
		static ModelDict LoadModels(ScenePointerType const& scene);

	// ----------------------------------------------------
	// Materials

	private:
		static MaterialGraphicPointerArrayType LoadMaterials(ScenePointerType const& scene);

		static MaterialGraphicPointerType LoadMaterial(ScenePointerType const& scene, std::size_t const materialIndex);

	// ----------------------------------------------------
	// Meshes

	private:
		static ResourceViewHeapPointerArrayType LoadMeshes(ScenePointerType const& scene);

		static ResourceViewHeapPointerType LoadMesh(ScenePointerType const& scene, std::size_t const meshIndex);

		static SrvToBufferPointerType LoadMeshIndices(ScenePointerType const& scene, std::size_t const meshIndex);

		static SrvToBufferPointerType LoadMeshVertices(ScenePointerType const& scene, std::size_t const meshIndex);

		static BoxedValuePointerArrayType LoadBuffer(ScenePointerType const& scene, std::size_t const bufferIndex);
	};
}

