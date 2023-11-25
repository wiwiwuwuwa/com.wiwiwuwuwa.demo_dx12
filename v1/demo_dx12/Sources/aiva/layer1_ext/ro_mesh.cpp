#include <pch.h>
#include <aiva/layer1_ext/ro_mesh.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/dx_dxgi_format_utils.h>
#include <aiva/layer1_ext/nr_buffer.h>
#include <aiva/layer1_ext/ro_mesh_gltf.h>
#include <aiva/layer1_ext/ro_scene_gltf.h>
#include <aiva/layer1_ext/ut_graphic_command_utils.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/enum_utils.h>
#include <aiva/utils_ext/e_value_type_utils.h>
#include <aiva/utils_ext/enum_utils.h>
#include <aiva/utils_ext/object_utils.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;
	using namespace aiva::utils_ext;

	RoMesh::RoMesh(aiva::layer1::Engine& engine) : mEngine{ engine }
	{

	}

	RoMesh::~RoMesh()
	{

	}

	void RoMesh::DeserealizeFromJson(nlohmann::json const& resourceJsn)
	{
		Asserts::CheckBool(!std::empty(resourceJsn), "Resource jsn is not valid");

		auto const meshData = NewObject<RoMeshGltf>(resourceJsn, mEngine);
		Asserts::CheckBool(meshData, "Mesh data is not valid");
		Asserts::CheckBool(!std::empty(meshData->IndicesData()), "Mesh indices data is not valid");
		Asserts::CheckBool(aiva::utils_ext::EnumUtils::IsValid(meshData->IndicesType()), "Mesh indices type is not valid");
		Asserts::CheckBool(!std::empty(meshData->VerticesData()), "Mesh vertices data is not valid");
		Asserts::CheckBool(meshData->VerticesStride() > std::size_t{}, "Mesh vertices stride is not valid");

		{
			mIndexCount = std::size(meshData->IndicesData()) / EValueTypeUtils::SizeOf(meshData->IndicesType());
			mVertexCount = std::size(meshData->VerticesData()) / meshData->VerticesStride();
		}

		{
			auto const indexBuffer = NewObject<NrBuffer>(mEngine);
			Asserts::CheckBool(indexBuffer, "Index buffer is not valid");

			indexBuffer->MemoryType(aiva::layer1::EBufferMemoryType::GpuOnly);
			indexBuffer->Size(std::size(meshData->IndicesData()));

			UtGraphicCommandUtils::CopyDataToBuffer(meshData->IndicesData(), indexBuffer, mEngine);
			mIndexBuffer = indexBuffer;

			auto const& indexResource = indexBuffer->Object();
			Asserts::CheckBool(indexResource, "Index resource is not valid");

			mNativeIndexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
			mNativeIndexBufferView.SizeInBytes = indexResource->GetDesc().Width;
			mNativeIndexBufferView.Format = DxDXGIFormatUtils::ParseFromValueType(meshData->IndicesType());
		}

		{
			auto const vertexBuffer = NewObject<NrBuffer>(mEngine);
			Asserts::CheckBool(vertexBuffer, "Vertex buffer is not valid");

			vertexBuffer->MemoryType(aiva::layer1::EBufferMemoryType::GpuOnly);
			vertexBuffer->Size(std::size(meshData->VerticesData()));

			UtGraphicCommandUtils::CopyDataToBuffer(meshData->VerticesData(), vertexBuffer, mEngine);
			mVertexBuffer = vertexBuffer;

			auto const& vertexResource = vertexBuffer->Object();
			Asserts::CheckBool(vertexResource, "Vertex resource is not valid");

			mNativeVertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
			mNativeVertexBufferView.SizeInBytes = vertexResource->GetDesc().Width;
			mNativeVertexBufferView.StrideInBytes = meshData->VerticesStride();
		}
	}

	std::vector<D3D12_RESOURCE_BARRIER> RoMesh::TransiteResourceBarriers(UtBarriersFlags const flags)
	{
		Asserts::CheckBool(mIndexBuffer, "Index buffer is not valid");
		Asserts::CheckBool(mVertexBuffer, "Vertex buffer is not valid");

		auto resourcesBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		if (aiva::utils::EnumUtils::Has(flags, UtBarriersFlags::Setup))
		{
			auto const indexBarriers = mIndexBuffer->TransiteResourceBarriers({}, D3D12_RESOURCE_STATE_INDEX_BUFFER);
			std::copy(std::cbegin(indexBarriers), std::cend(indexBarriers), std::back_inserter(resourcesBarriers));

			auto const vertexBarriers = mVertexBuffer->TransiteResourceBarriers({}, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			std::copy(std::cbegin(vertexBarriers), std::cend(vertexBarriers), std::back_inserter(resourcesBarriers));
		}

		if (aiva::utils::EnumUtils::Has(flags, UtBarriersFlags::Reset))
		{
			auto const indexBarriers = mIndexBuffer->TransiteResourceBarriers({}, D3D12_RESOURCE_STATE_COMMON);
			std::copy(std::cbegin(indexBarriers), std::cend(indexBarriers), std::back_inserter(resourcesBarriers));

			auto const vertexBarriers = mVertexBuffer->TransiteResourceBarriers({}, D3D12_RESOURCE_STATE_COMMON);
			std::copy(std::cbegin(vertexBarriers), std::cend(vertexBarriers), std::back_inserter(resourcesBarriers));
		}

		return resourcesBarriers;
	}
}
