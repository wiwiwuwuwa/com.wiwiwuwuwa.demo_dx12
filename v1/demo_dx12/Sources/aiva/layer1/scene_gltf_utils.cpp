#include <pch.h>
#include <aiva/layer1/scene_gltf_utils.h>

#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/grv_srv_to_buffer.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/layer1/ro_scene_gltf.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/boxed_type_utils.h>
#include <aiva/utils/boxed_value_utils.h>
#include <aiva/utils/dict_buffer.h>
#include <aiva/utils/dict_struct.h>
#include <aiva/utils/e_boxed_type.h>
#include <aiva/utils/gltf_type_desc.h>
#include <aiva/utils/i_boxed_value.h>
#include <aiva/utils/material_constants.h>
#include <aiva/utils/meta_field.h>
#include <aiva/utils/meta_struct.h>
#include <aiva/utils/object_utils.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	SceneGltfUtils::ModelDict SceneGltfUtils::LoadModels(ScenePointerType const& scene)
	{
		auto const materials = LoadMaterials(scene);
		auto const meshes = LoadMeshes(scene);

		auto models = ModelDict{};

		for (std::size_t i = {}; i < std::size(scene->Model().nodes); i++)
		{
			auto const& glNode = scene->Model().nodes.at(i);
			if (glNode.mesh == -1)
			{
				continue;
			}

			auto const& glMesh = scene->Model().meshes.at(glNode.mesh);
			auto const& glPrimitive = glMesh.primitives.at(0);

			auto const& material = materials.at(glPrimitive.material);
			auto const& mesh = meshes.at(glNode.mesh);

			auto const model = material->Copy();
			Asserts::CheckBool(model, "Model is not valid");

			auto const resourceHeap = model->ResourceDescriptor().ResourceTable().GetOrAddResourceHeap(mesh->HeapType());
			Asserts::CheckBool(resourceHeap, "Resource heap is not valid");

			for (auto const& resourceView : mesh->GetViews())
			{
				resourceHeap->SetView(resourceView.first, resourceView.second);
			}

			models.insert_or_assign(i, model);
		}

		return models;
	}

	SceneGltfUtils::MaterialGraphicPointerArrayType SceneGltfUtils::LoadMaterials(ScenePointerType const& scene)
	{
		Asserts::CheckBool(scene, "Scene is not valid");

		auto materials = MaterialGraphicPointerArrayType{};

		for (std::size_t i = {}; i < std::size(scene->Model().materials); i++)
		{
			auto const material = LoadMaterial(scene, i);
			Asserts::CheckBool(material, "Material is not valid");

			materials.emplace_back(material);
		}

		return materials;
	}

	SceneGltfUtils::MaterialGraphicPointerType SceneGltfUtils::LoadMaterial(ScenePointerType const& scene, std::size_t const materialIndex)
	{
		Asserts::CheckBool(scene, "Scene is not valid");

		auto const& glMaterial = scene->Model().materials.at(materialIndex);
		auto const& glMaterialPath = glMaterial.extras.Get("path");

		auto const& materialPath = glMaterialPath.Get<std::string>();
		auto const material = scene->Engine().ResourceSystem().GetResource<RoMaterialGraphic>(materialPath);

		Asserts::CheckBool(material, "Material is not valid");
		return material;
	}

	SceneGltfUtils::ResourceViewHeapPointerArrayType SceneGltfUtils::LoadMeshes(ScenePointerType const& scene)
	{
		Asserts::CheckBool(scene, "Scene is not valid");

		auto meshes = ResourceViewHeapPointerArrayType{};

		for (std::size_t i = {}; i < std::size(scene->Model().meshes); i++)
		{
			auto const mesh = LoadMesh(scene, i);
			Asserts::CheckBool(mesh, "Mesh is not valid");

			meshes.emplace_back(mesh);
		}

		return meshes;
	}

	SceneGltfUtils::ResourceViewHeapPointerType SceneGltfUtils::LoadMesh(ScenePointerType const& scene, std::size_t const meshIndex)
	{
		Asserts::CheckBool(scene, "Scene is not valid");

		auto const meshHeap = NewObject<ResourceViewHeapElementType>(scene->Engine(), EDescriptorHeapType::CbvSrvUav);
		Asserts::CheckBool(meshHeap, "Mesh is not valid");

		auto const indicesBuffer = LoadMeshIndices(scene, meshIndex);
		Asserts::CheckBool(indicesBuffer, "Indices buffer is not valid");

		meshHeap->SetView(MaterialConstants::AIVA_BUFFER_MESH_INDICES, indicesBuffer);

		auto const verticesBuffer = LoadMeshVertices(scene, meshIndex);
		Asserts::CheckBool(verticesBuffer, "Vertices buffer is not valid");

		meshHeap->SetView(MaterialConstants::AIVA_BUFFER_MESH_VERTICES, verticesBuffer);

		return meshHeap;
	}

	SceneGltfUtils::SrvToBufferPointerType SceneGltfUtils::LoadMeshIndices(ScenePointerType const& scene, std::size_t const meshIndex)
	{
		static constexpr char const* const FIELD_VERTEX_ID_NAME = "VERTEX_ID";

		Asserts::CheckBool(scene, "Scene is not valid");

		auto const& glMesh = scene->Model().meshes.at(meshIndex);
		auto const& glPrimitive = glMesh.primitives.at(0);

		auto const meshIndices = NewObject<SrvToBufferElementType>(scene->Engine());
		Asserts::CheckBool(meshIndices, "Mesh indices is not valid");

		{ // Generate Meta Struct
			auto const metaStruct = NewObject<MetaStructElementType>();
			Asserts::CheckBool(metaStruct, "Meta struct is not valid");

			auto const metaField = NewObject<MetaFieldElementType>();
			Asserts::CheckBool(metaField, "Meta field is not valid");

			metaField->Name(FIELD_VERTEX_ID_NAME);
			metaField->Type(EBoxedType::UINT32);
			metaStruct->Add(metaField);

			meshIndices->Buffer().Layout(metaStruct);
		}

		{ // Generate Data Structs
			auto const indicesBuffer = LoadBuffer(scene, glPrimitive.indices);

			for (auto const& indexValue : indicesBuffer)
			{
				auto const dictStruct = NewObject<DictStructElementType>();
				Asserts::CheckBool(dictStruct, "Dict struct is not valid");

				auto const dictField = BoxedValueUtils::CastTo(indexValue, EBoxedType::UINT32);
				Asserts::CheckBool(dictField, "Dict field is not valid");

				dictStruct->FieldBoxed(FIELD_VERTEX_ID_NAME, dictField);
				meshIndices->Buffer().Add(dictStruct);
			}
		}

		meshIndices->GetOrAddInternalResource();
		return meshIndices;
	}

	SceneGltfUtils::SrvToBufferPointerType SceneGltfUtils::LoadMeshVertices(ScenePointerType const& scene, std::size_t const meshIndex)
	{
		Asserts::CheckBool(scene, "Scene is not valid");

		auto const& glMesh = scene->Model().meshes.at(meshIndex);
		auto const& glPrimitive = glMesh.primitives.at(0);

		auto const meshVertices = NewObject<SrvToBufferElementType>(scene->Engine());
		Asserts::CheckBool(meshVertices, "Mesh indices is not valid");

		{ // Generate Meta Struct
			auto const metaStruct = NewObject<MetaStructElementType>();
			Asserts::CheckBool(metaStruct, "Meta struct is not valid");

			for (auto const& glAttribute : glPrimitive.attributes)
			{
				auto const& glSemantic = glAttribute.first;
				auto const& glAccessor = scene->Model().accessors.at(glAttribute.second);

				auto glType = GltfTypeDesc{};
				glType.TypeOfComponent = glAccessor.type;
				glType.TypeInComponent = glAccessor.componentType;
				auto const fieldType = BoxedTypeUtils::Parse(glType);

				auto const metaField = NewObject<MetaFieldElementType>();
				Asserts::CheckBool(metaField, "Meta field is not valid");

				metaField->Name(glSemantic);
				metaField->Type(fieldType);
				metaStruct->Add(metaField);
			}

			meshVertices->Buffer().Layout(metaStruct);
		}

		{ // Generate Data Structs
			auto dictStructs = std::map<std::size_t, DictStructPointerType>{};

			for (auto const& glAttribute : glPrimitive.attributes)
			{
				auto const& glSemantic = glAttribute.first;
				auto const& valuesBuffer = LoadBuffer(scene, glAttribute.second);

				for (std::size_t i = {}; i < std::size(valuesBuffer); i++)
				{
					auto dictIter = dictStructs.find(i);
					if (dictIter == std::end(dictStructs))
					{
						dictIter = dictStructs.insert_or_assign(i, NewObject<DictStructElementType>()).first;
					}

					auto const& dictStruct = dictIter->second;
					Asserts::CheckBool(dictStruct, "Dict struct is not valid");

					auto const& dictField = valuesBuffer.at(i);
					Asserts::CheckBool(dictField, "Dict field is not valid");

					dictStruct->FieldBoxed(glSemantic, dictField);
				}
			}

			for (auto const& dictStruct : dictStructs)
			{
				meshVertices->Buffer().Add(dictStruct.second);
			}
		}

		meshVertices->GetOrAddInternalResource();
		return meshVertices;
	}

	SceneGltfUtils::BoxedValuePointerArrayType SceneGltfUtils::LoadBuffer(ScenePointerType const& scene, std::size_t const bufferIndex)
	{
		Asserts::CheckBool(scene, "Scene is not valid");

		auto const& glAccessor = scene->Model().accessors.at(bufferIndex);
		auto const& glBufferView = scene->Model().bufferViews.at(glAccessor.bufferView);
		auto const& glBuffer = scene->Model().buffers.at(glBufferView.buffer);

		auto glType = GltfTypeDesc{};
		glType.TypeOfComponent = glAccessor.type;
		glType.TypeInComponent = glAccessor.componentType;

		auto const valueType = BoxedTypeUtils::Parse(glType);
		auto const valueSize = BoxedValueUtils::SizeOf(valueType);
		auto const valueStride = std::max(glBufferView.byteStride, valueSize);

		auto bufferBinary = boost::as_bytes(boost::span{ glBuffer.data });
		bufferBinary = bufferBinary.subspan(glBufferView.byteOffset, glBufferView.byteLength);
		bufferBinary = bufferBinary.last(std::size(bufferBinary) - glAccessor.byteOffset);

		auto bufferValues = BoxedValuePointerArrayType{};
		for (std::size_t i = {}; i < std::size_t{ glAccessor.count }; i++)
		{
			auto const valueBinary = bufferBinary.subspan(valueStride * i, valueSize);
			Asserts::CheckBool(!std::empty(valueBinary), "Value binary is empty");

			auto const valueObject = BoxedValueUtils::DeserealizeFromBinary(valueBinary, valueType);
			Asserts::CheckBool(valueObject, "Value object is not valid");

			bufferValues.emplace_back(valueObject);
		}

		return bufferValues;
	}
}
