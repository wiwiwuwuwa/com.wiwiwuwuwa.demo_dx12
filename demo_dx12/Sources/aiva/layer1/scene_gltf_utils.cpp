#include <pch.h>
#include <aiva/layer1/scene_gltf_utils.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/e_resource_memory_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/grv_srv_to_buffer.h>
#include <aiva/layer1/i_shader_value.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/layer1/ro_scene_gltf.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/layer1/shader_value_utils.h>
#include <aiva/utils/material_constants.h>
#include <aiva/utils/object_factory.h>

aiva::layer1::SceneGltfUtils::SceneGltfUtils()
{

}

aiva::layer1::SceneGltfUtils::~SceneGltfUtils()
{

}

aiva::layer1::SceneGltfUtils& aiva::layer1::SceneGltfUtils::Instance()
{
	static SceneGltfUtils instance{};
	return instance;
}

aiva::layer1::SceneGltfUtils::MaterialPerNodeMap aiva::layer1::SceneGltfUtils::LoadMaterials(RoSceneGltf const& gltf)
{
	auto const graphicMaterials = LoadGraphicMaterials(gltf);
	auto const meshesMaterials = LoadMeshMaterials(gltf);
	auto const nodesMaterials = LoadNodeMaterials(gltf, graphicMaterials, meshesMaterials);

	return nodesMaterials;
}

aiva::layer1::SceneGltfUtils::MaterialArray aiva::layer1::SceneGltfUtils::LoadGraphicMaterials(RoSceneGltf const& gltf)
{
	auto const& gltfMaterials = gltf.Model().materials;
	auto aivaMaterials = std::vector<std::shared_ptr<aiva::layer1::RoMaterialGraphic>>{};

	for (std::size_t i = {}; i < gltfMaterials.size(); i++)
	{
		auto const& gltfMaterial = gltfMaterials.at(i);
		auto const& gltfMaterialPath = gltfMaterial.extras.Get("path");

		auto const& aivaMaterialPath = gltfMaterialPath.Get<std::string>();
		auto const& aivaMaterial = gltf.Engine().ResourceSystem().GetResource<RoMaterialGraphic>(aivaMaterialPath);

		aivaMaterials.emplace_back(aivaMaterial);
	}

	return aivaMaterials;
}

aiva::layer1::SceneGltfUtils::MeshArray aiva::layer1::SceneGltfUtils::LoadMeshMaterials(RoSceneGltf const& gltf)
{
	auto const& gltfMeshes = gltf.Model().meshes;
	auto aivaMeshes = std::vector<std::shared_ptr<aiva::layer1::ResourceViewHeap>>();

	for (std::size_t i = {}; i < gltfMeshes.size(); i++)
	{
		auto const& gltfMesh = gltfMeshes.at(i);
		auto const& gltfPrimitive = gltfMesh.primitives.at(0);

		auto const aivaMesh = ResourceViewHeap::FactoryType::Create<ResourceViewHeap>(gltf.Engine(), EDescriptorHeapType::CbvSrvUav);
		aivaMeshes.emplace_back(aivaMesh);

		{ // indices
			static auto const INDEX_KEY = "m0_INDEX";

			auto const aivaView = GrvSrvToBuffer::FactoryType::Create<GrvSrvToBuffer>(gltf.Engine());
			aivaMesh->SetView(aiva::utils::MaterialConstants::AIVA_BUFFER_INDICES, aivaView);

			{
				auto const aivaBuffer = GrBuffer::FactoryType::Create<GrBuffer>(gltf.Engine());
				aivaBuffer->MemoryType(EResourceMemoryType::CpuToGpu);

				aivaView->InternalResource(aivaBuffer);
			}

			{
				auto const& gltfAccessor = gltf.Model().accessors.at(gltfPrimitive.indices);

				auto const aivaStruct = ShaderStruct::FactoryType::Create<ShaderStruct>();
				auto const aivaStructValue = ShaderValueUtils::CreateFromGltf(gltfAccessor.type, gltfAccessor.componentType);
				aivaStruct->SetValue(INDEX_KEY, &aivaStructValue);

				aivaView->Buffer().Struct(aivaStruct);
			}

			{
				auto const aivaValues = LoadBufferByAccessor(gltf, gltfPrimitive.indices);
				for (auto const& aivaValue : aivaValues)
				{
					auto const aivaStruct = ShaderStruct::FactoryType::Create<ShaderStruct>();
					aivaStruct->SetStruct(INDEX_KEY, &aivaValue);

					aivaView->Buffer().Add(aivaStruct);
				}
			}
		}

		{ // vertices
			auto const aivaView = GrvSrvToBuffer::FactoryType::Create<GrvSrvToBuffer>(gltf.Engine());
			aivaMesh->SetView(aiva::utils::MaterialConstants::AIVA_BUFFER_VERTICES, aivaView);

			{
				auto const aivaBuffer = GrBuffer::FactoryType::Create<GrBuffer>(gltf.Engine());
				aivaBuffer->MemoryType(EResourceMemoryType::CpuToGpu);

				aivaView->InternalResource(aivaBuffer);
			}

			{
				auto const aivaStruct = ShaderStruct::FactoryType::Create<ShaderStruct>();

				for (auto const& gltfAttribute : gltfPrimitive.attributes)
				{
					auto const& gltfSemantic = gltfAttribute.first;
					auto const& gltfAccessor = gltf.Model().accessors.at(gltfAttribute.second);

					auto const aivaStructValue = ShaderValueUtils::CreateFromGltf(gltfAccessor.type, gltfAccessor.componentType);
					aivaStruct->SetValue(gltfSemantic, &aivaStructValue);
				}

				aivaView->Buffer().Struct(aivaStruct);
			}

			{
				auto aivaStructs = std::vector<std::shared_ptr<ShaderStruct>>{};

				{ // first pass: create empty structures
					auto const& gltfAttribute = *std::cbegin(gltfPrimitive.attributes);
					auto const& gltfAccessor = gltf.Model().accessors.at(gltfAttribute.second);
					auto const& gltfVerticesCount = gltfAccessor.count;

					for (std::size_t i = {}; i < gltfVerticesCount; i++)
					{
						auto const aivaStruct = ShaderStruct::FactoryType::Create<ShaderStruct>();
						aivaStructs.emplace_back(aivaStruct);
					}
				}

				{ // second pass: merge structs
					for (std::size_t attributeID = {}; attributeID < std::size(gltfPrimitive.attributes); attributeID++)
					{
						auto const& gltfAttribute = *std::next(std::cbegin(gltfPrimitive.attributes), attributeID);
						auto const& gltfSemantic = gltfAttribute.first;

						auto const aivaValues = LoadBufferByAccessor(gltf, gltfAttribute.second);
						for (std::size_t structureID = {}; structureID < std::size(aivaStructs); structureID++)
						{
							auto const& aivaStruct = aivaStructs.at(structureID);
							auto const& aivaValue = aivaValues.at(structureID);
							aivaStruct->SetStruct(gltfSemantic, &aivaValue);
						}
					}
				}

				{ // third pass: fill srv buffer
					for (auto const& aivaStruct : aivaStructs)
					{
						aivaView->Buffer().Add(aivaStruct);
					}
				}
			}
		}
	}

	return aivaMeshes;
}

aiva::layer1::SceneGltfUtils::VertexArray aiva::layer1::SceneGltfUtils::LoadBufferByAccessor(RoSceneGltf const& gltf, std::size_t const accessorID)
{
	auto const&const gltfAccessor = gltf.Model().accessors.at(accessorID);
	auto const&const gltfBufferView = gltf.Model().bufferViews.at(gltfAccessor.bufferView);
	auto const&const gltfBuffer = gltf.Model().buffers.at(gltfBufferView.buffer);

	auto bufferBinary = boost::as_bytes(boost::span{ gltfBuffer.data });
	bufferBinary = bufferBinary.subspan(gltfBufferView.byteOffset, gltfBufferView.byteLength);
	bufferBinary = bufferBinary.last(std::size(bufferBinary) - gltfAccessor.byteOffset);

	auto shaderValues = std::vector<std::shared_ptr<IShaderValue>>{};
	for (std::size_t vectorID = {}; vectorID < std::size_t{ gltfAccessor.count }; vectorID++)
	{
		auto const shaderValue = ShaderValueUtils::CreateFromGltf(gltfAccessor.type, gltfAccessor.componentType);
		shaderValues.emplace_back(shaderValue);

		auto const bytesInStride = std::max(gltfBufferView.byteStride, shaderValue->MinBinarySize());
		auto const strideBinary = bufferBinary.subspan(vectorID * bytesInStride, bytesInStride);

		shaderValue->DeserealizeFromBinary(strideBinary);
	}

	return shaderValues;
}

aiva::layer1::SceneGltfUtils::MaterialPerNodeMap aiva::layer1::SceneGltfUtils::LoadNodeMaterials(RoSceneGltf const& gltf, MaterialArray const& materials, MeshArray const& meshes)
{
	auto materialsPerNodes = MaterialPerNodeMap{};

	for (std::size_t nodeID{}; nodeID < std::size(gltf.Model().nodes); nodeID++)
	{
		auto const& gltfNode = gltf.Model().nodes.at(nodeID);
		if (gltfNode.mesh == -1)
		{
			continue;
		}

		auto const& gltfMesh = gltf.Model().meshes.at(gltfNode.mesh);
		auto const& gltfPrimitive = gltfMesh.primitives.at(0);

		auto const& aivaMaterial = materials.at(gltfPrimitive.material);
		auto const& aivaMesh = meshes.at(gltfNode.mesh);

		auto const materialInstance = aivaMaterial->Copy();
		materialsPerNodes.insert_or_assign(nodeID, materialInstance);

		auto materialHeap = materialInstance->ResourceDescriptor().ResourceTable().GetResourceHeap(aivaMesh->HeapType());
		if (!materialHeap)
		{
			materialHeap = ResourceViewHeap::FactoryType::Create<ResourceViewHeap>(gltf.Engine(), aivaMesh->HeapType());
			materialInstance->ResourceDescriptor().ResourceTable().SetResourceHeap(aivaMesh->HeapType(), materialHeap);
		}

		for (auto const& resourceView : aivaMesh->GetViews())
		{
			materialHeap->SetView(resourceView.first, resourceView.second);
		}
	}

	return materialsPerNodes;
}
