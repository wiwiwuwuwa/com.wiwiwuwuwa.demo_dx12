#include <pch.h>
#include <aiva/layer1_ext/gc_draw_mesh.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1_ext/graphic_hardware.h>
#include <aiva/layer1_ext/ro_material_gr.h>
#include <aiva/layer1_ext/ro_mesh.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1_ext
{
	using namespace aiva::utils;

	GcDrawMesh::GcDrawMesh(aiva::layer1::Engine& engine) : mEngine{ engine }
	{
		OnExec().AttachListener(&ThisType::GraphicCommand_OnExec, this);
	}

	GcDrawMesh::~GcDrawMesh()
	{
		OnExec().RemoveListener(&ThisType::GraphicCommand_OnExec, this);
	}

	void GcDrawMesh::GraphicCommand_OnExec()
	{
		ExecuteResourceBarrier();
		ExecuteSetPipelineState();
		ExecuteSetGraphicRootSignature();
		ExecuteSetDescriptorHeaps();
		ExecuteSetGraphicsRootDescriptorTable();
		ExecuteIASetPrimitiveTopology();
		ExecuteIASetIndexBuffer();
		ExecuteIASetVertexBuffers();
		ExecuteDrawIndexedInstanced();
	}

	void GcDrawMesh::ExecuteResourceBarrier()
	{
		auto resourcesBarriers = std::vector<D3D12_RESOURCE_BARRIER>{};

		{
			auto const& geometryObject = mGeometry;
			Asserts::CheckBool(geometryObject, "Geometry object is not valid");

			auto const geometryBarriers = geometryObject->TransiteResourceBarriers(UtBarriersFlags::Setup);
			std::copy(std::cbegin(geometryBarriers), std::cend(geometryBarriers), std::back_inserter(resourcesBarriers));
		}

		{
			auto const& materialObject = mMaterial;
			Asserts::CheckBool(materialObject, "Material object is not valid");

			auto const materialBarriers = materialObject->TransiteResourceBarriers(UtBarriersFlags::Setup);
			std::copy(std::cbegin(materialBarriers), std::cend(materialBarriers), std::back_inserter(resourcesBarriers));
		}

		if (std::empty(resourcesBarriers))
		{
			return;
		}

		mEngine.GraphicHardwareExt().CommandList().ResourceBarrier(std::size(resourcesBarriers), std::data(resourcesBarriers));
	}

	void GcDrawMesh::ExecuteSetPipelineState()
	{
		auto const& materialObject = mMaterial;
		Asserts::CheckBool(materialObject, "Material object is not valid");

		auto const& pipelineState = materialObject->NativePipelineState();
		Asserts::CheckBool(pipelineState, "Pipeline state is not valid");

		mEngine.GraphicHardwareExt().CommandList().SetPipelineState(pipelineState.get());
	}

	void GcDrawMesh::ExecuteSetGraphicRootSignature()
	{
		auto const& materialObject = mMaterial;
		Asserts::CheckBool(materialObject, "Material object is not valid");

		auto const& rootSignature = materialObject->NativeRootSignature();
		Asserts::CheckBool(rootSignature, "Root signature is not valid");

		mEngine.GraphicHardwareExt().CommandList().SetGraphicsRootSignature(rootSignature.get());
	}

	void GcDrawMesh::ExecuteSetDescriptorHeaps()
	{
		auto const& materialObject = mMaterial;
		Asserts::CheckBool(mMaterial, "Material object is not valid");

		auto const descriptorHeapsObj = materialObject->NativeDescriptorHeaps();

		auto descriptorHeapsPtr = std::vector<ID3D12DescriptorHeap*>{};
		for (auto const& obj : descriptorHeapsObj)
		{
			Asserts::CheckBool(obj, "Obj is not valid");

			auto const ptr = obj.get();
			Asserts::CheckBool(ptr, "Ptr is not valid");

			descriptorHeapsPtr.push_back(ptr);
		}

		mEngine.GraphicHardwareExt().CommandList().SetDescriptorHeaps(std::size(descriptorHeapsPtr), std::data(descriptorHeapsPtr));
	}

	void GcDrawMesh::ExecuteSetGraphicsRootDescriptorTable()
	{
		auto const& materialObject = mMaterial;
		Asserts::CheckBool(mMaterial, "Material object is not valid");

		auto const descriptorHeapsObj = materialObject->NativeDescriptorHeaps();

		for (auto i = std::size_t{}; i < std::size(descriptorHeapsObj); i++)
		{
			auto const& obj = descriptorHeapsObj.at(i);
			Asserts::CheckBool(obj, "Obj is not valid");

			auto const hnd = obj->GetGPUDescriptorHandleForHeapStart();
			mEngine.GraphicHardwareExt().CommandList().SetGraphicsRootDescriptorTable(i, hnd);
		}
	}

	void GcDrawMesh::ExecuteIASetPrimitiveTopology()
	{
		mEngine.GraphicHardwareExt().CommandList().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void GcDrawMesh::ExecuteIASetIndexBuffer()
	{
		auto const& geometryObject = mGeometry;
		Asserts::CheckBool(geometryObject, "Geometry object is not valid");

		mEngine.GraphicHardwareExt().CommandList().IASetIndexBuffer(&geometryObject->NativeIndexBufferView());
	}

	void GcDrawMesh::ExecuteIASetVertexBuffers()
	{
		auto const& geometryObject = mGeometry;
		Asserts::CheckBool(geometryObject, "Geometry object is not valid");

		mEngine.GraphicHardwareExt().CommandList().IASetVertexBuffers({}, 1, &geometryObject->NativeVertexBufferView());
	}

	void GcDrawMesh::ExecuteDrawIndexedInstanced()
	{
		auto const& geometryObject = mGeometry;
		Asserts::CheckBool(geometryObject, "Geometry object is not valid");

		auto const instancesCount = mInstance;
		Asserts::CheckBool(instancesCount, "Instances count is not valid");

		mEngine.GraphicHardwareExt().CommandList().DrawIndexedInstanced(geometryObject->IndexCount(), instancesCount, {}, {}, {});
	}
}
