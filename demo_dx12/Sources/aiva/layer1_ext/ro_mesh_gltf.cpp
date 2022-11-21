#include <pch.h>
#include <aiva/layer1_ext/ro_mesh_gltf.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/gl_buffer_view_for_indices.h>
#include <aiva/layer1_ext/gl_buffer_view_for_vertices.h>
#include <aiva/layer1_ext/resource_system.h>
#include <aiva/layer1_ext/ro_scene_gltf.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	RoMeshGltf::RoMeshGltf(aiva::layer1::Engine& engine) : mEngine{ engine }
	{

	}

	RoMeshGltf::RoMeshGltf(nlohmann::json const& resourceJsn, aiva::layer1::Engine& engine) : RoMeshGltf{ engine }
	{
		DeserealizeFromJson(resourceJsn);
	}

	RoMeshGltf::~RoMeshGltf()
	{

	}

	void RoMeshGltf::DeserealizeFromJson(nlohmann::json const& resourceJsn)
	{
		Asserts::CheckBool(!std::empty(resourceJsn), "Resource jsn is not valid");

		auto const resource = mEngine.ResourceSystemExt().GetResource<RoSceneGltf>(resourceJsn.at("binary_path").get<std::string>());
		Asserts::CheckBool(resource, "Resource is not valid");

		auto const& meshes = resource->Data().meshes;
		Asserts::CheckBool(!std::empty(meshes), "Meshes are not valid");

		auto const meshName = resourceJsn.at("mesh_name").get<std::string>();
		Asserts::CheckBool(!std::empty(meshName), "Mesh name is not valid");

		auto const meshIter = std::find_if(std::cbegin(meshes), std::cend(meshes), [&](auto const& mesh) { return mesh.name == meshName; });
		Asserts::CheckBool(meshIter != std::cend(meshes), "Mesh iter is not valid");

		auto const meshIndex = static_cast<std::size_t>(std::distance(std::cbegin(meshes), meshIter));
		Asserts::CheckBool(meshIndex >= decltype(meshIndex) {} && meshIndex < std::size(meshes), "Mesh index is not valid");
		auto const& mesh = meshes.at(meshIndex);

		auto const& primitives = mesh.primitives;
		Asserts::CheckBool(!std::empty(primitives), "Primitives are not valid");

		auto const primitiveIndex = resourceJsn.at("primitive_index").get<std::size_t>();
		Asserts::CheckBool(primitiveIndex >= decltype(primitiveIndex){} && primitiveIndex < std::size(primitives), "Primitive index is not valid");

		auto const bufferViewForIndices = GlBufferViewForIndices{ resource->Data(), meshIndex, primitiveIndex };
		mIndicesData = bufferViewForIndices.GetData();
		mIndicesType = bufferViewForIndices.TypeOfElement();

		auto const bufferViewForVertices = GlBufferViewForVertices{ resource->Data(), meshIndex, primitiveIndex, mEngine };
		mVerticesData = bufferViewForVertices.GetData();
		mVerticesStride = bufferViewForVertices.SizeOfElement();
	}
}
