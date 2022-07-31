#include <pch.h>
#include <aiva/layer1/scene_gltf_utils.h>

#include <aiva/layer1/e_gpu_descriptor_heap_type.h>
#include <aiva/layer1/e_gpu_resource_memory_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/grv_srv_to_buffer.h>
#include <aiva/layer1/i_shader_value.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/layer1/ro_scene_gltf.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/layer1/shader_value_utils.h>

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

std::vector<std::shared_ptr<aiva::layer1::RoMaterialGraphic>> aiva::layer1::SceneGltfUtils::LoadGraphicMaterials(RoSceneGltf const& gltf)
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

std::vector<std::shared_ptr<aiva::layer1::ResourceViewHeap>> aiva::layer1::SceneGltfUtils::LoadMeshMaterials(RoSceneGltf const& gltf)
{
	auto const& gltfMeshes = gltf.Model().meshes;
	auto aivaMeshes = std::vector<std::shared_ptr<aiva::layer1::ResourceViewHeap>>();

	for (std::size_t i = {}; i < gltfMeshes.size(); i++)
	{
		auto const& gltfMesh = gltfMeshes.at(i);
		auto const& gltfPrimitive = gltfMesh.primitives.at(0);

		auto const aivaMesh = ResourceViewHeap::Create(gltf.Engine(), EGpuDescriptorHeapType::CbvSrvUav);
		aivaMeshes.emplace_back(aivaMesh);

		{ // Indices
			static auto const INDEX_KEY = "m0_INDEX";
			static auto const INDICES_KEY = "m0_INDICES";

			auto aivaViewDesc = GrvSrvToBufferDesc{};
			{
				{
					auto aivaBufferDesc = GrBufferDesc{};
					aivaBufferDesc.MemoryType = EGpuResourceMemoryType::CpuToGpu;

					auto const aivaBuffer = GrBuffer::Create(gltf.Engine(), aivaBufferDesc);
					aivaViewDesc.Resource = aivaBuffer;
				}

				{
					auto const& gltfAccessor = gltf.Model().accessors.at(gltfPrimitive.indices);

					auto const aivaRefStruct = ShaderStruct::Create();
					aivaViewDesc.Struct = aivaRefStruct;

					auto const aivaRefValue = ShaderValueUtils::CreateFromGltf(gltfAccessor.type, gltfAccessor.componentType);
					aivaRefStruct->SetValue(INDEX_KEY, &aivaRefValue);
				}
			}

			auto const aivaView = GrvSrvToBuffer::Create(gltf.Engine(), aivaViewDesc);
			aivaMesh->ResourceView(INDICES_KEY, aivaView);

			{
				auto const aivaValues = LoadBufferByAccessor(gltf, gltfPrimitive.indices);
				for (auto const& aivaValue : aivaValues)
				{
					auto const aivaStruct = ShaderStruct::Create();
					aivaStruct->SetStruct(INDEX_KEY, &aivaValue);

					aivaView->Buffer().Add(aivaStruct);
				}
			}
		}
	}

	return aivaMeshes;
}

std::vector<std::shared_ptr<aiva::layer1::IShaderValue>> aiva::layer1::SceneGltfUtils::LoadBufferByAccessor(RoSceneGltf const& gltf, std::size_t const accessorID)
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
