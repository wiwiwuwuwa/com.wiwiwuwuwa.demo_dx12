#include <pch.h>
#include <aiva/layer2/render_system.h>

#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/e_primitive_topology.h>
#include <aiva/layer1/e_resource_buffer_format.h>
#include <aiva/layer1/e_resource_memory_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gca_clear_depth_stencil.h>
#include <aiva/layer1/gca_clear_render_target.h>
#include <aiva/layer1/gca_draw_mesh.h>
#include <aiva/layer1/gca_present.h>
#include <aiva/layer1/gca_set_render_target.h>
#include <aiva/layer1/gca_set_scissor_rects.h>
#include <aiva/layer1/gca_set_viewports.h>
#include <aiva/layer1/graphic_executor.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/graphic_pipeline.h>
#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/layer1/grv_cbv_to_buffer.h>
#include <aiva/layer1/grv_dsv_to_texture_2d.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>
#include <aiva/layer1/grv_srv_to_buffer.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/layer2/sc_camera.h>
#include <aiva/layer2/sc_mesh_renderer.h>
#include <aiva/layer2/scene_actor.h>
#include <aiva/layer2/world.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/dict_struct.h>
#include <aiva/utils/material_constants.h>
#include <aiva/utils/object_utils.h>

namespace aiva::layer2
{
	using namespace aiva::layer1;
	using namespace aiva::utils;

	RenderSystem::RenderSystem(World const& world) : AObject{}, mWorld{ world }
	{
		InitializeRenderer();
	}

	RenderSystem::~RenderSystem()
	{
		TerminateRenderer();
	}

	RenderSystem::EvPopulateCameras& RenderSystem::OnPopulateCameras()
	{
		return mOnPopulateCameras;
	}

	RenderSystem::EvPopulateMeshRenderers& RenderSystem::OnPopulateMeshRenderers()
	{
		return mOnPopulateMeshRenderers;
	}

	void RenderSystem::InitializeRenderer()
	{
		mWorld.Engine().GraphicPipeline().OnPopulateCommands().connect(boost::bind(&RenderSystem::ExecuteRenderer, this));
		InitRTs();
		InitDSs();
	}

	void RenderSystem::TerminateRenderer()
	{
		ShutDSs();
		ShutRTs();
		mWorld.Engine().GraphicPipeline().OnPopulateCommands().disconnect(boost::bind(&RenderSystem::ExecuteRenderer, this));
	}

	void RenderSystem::ExecuteRenderer()
	{
		UseRTsDSs();
		ClearRTs();
		ClearDSs();
		UseViewports();
		UseScissorRects();

		auto const cameras = OnPopulateCameras()();
		auto const meshRenderers = OnPopulateMeshRenderers()();

		for (auto const& const camera : cameras)
		{
			Asserts::CheckBool(camera, "Camera is not valid");
			for (auto const& const meshRenderer : meshRenderers)
			{
				Asserts::CheckBool(meshRenderer, "Mesh renderer is not valid");
				DrawMeshRenderer(*camera, *meshRenderer);
			}
		}

		PresentST();
	}

	void RenderSystem::InitRTs()
	{
		auto const viewRect = mWorld.Engine().GraphicHardware().ScreenViewRect();

		mRTs = NewObject<ResourceViewHeapType>(mWorld.Engine());
		mRTs->HeapType(EDescriptorHeapType::Rtv);

		for (std::size_t i = {}; i < std::size_t{ NUM_DEFFERED_BUFFERS }; i++)
		{
			auto texBuffer = NewObject<GrTexture2D>(mWorld.Engine());
			texBuffer->Format(EResourceBufferFormat::R32G32B32A32_FLOAT);
			texBuffer->Width(viewRect.z);
			texBuffer->Height(viewRect.w);
			texBuffer->SupportRenderTarget(true);
			texBuffer->SupportUnorderedAccess(true);

			auto texView = NewObject<GrvRtvToTexture2D>(mWorld.Engine());
			texView->SetInternalResource(texBuffer);

			mRTs->SetView(std::to_string(i), texView);
		}
	}

	void RenderSystem::InitDSs()
	{
		auto const viewRect = mWorld.Engine().GraphicHardware().ScreenViewRect();

		mDSs = NewObject<ResourceViewHeapType>(mWorld.Engine());
		mDSs->HeapType(EDescriptorHeapType::Dsv);

		auto texBuffer = NewObject<GrTexture2D>(mWorld.Engine());
		texBuffer->Format(EResourceBufferFormat::D32_FLOAT);
		texBuffer->Width(viewRect.z);
		texBuffer->Height(viewRect.w);
		texBuffer->SupportDepthStencil(true);

		auto texView = NewObject<GrvDsvToTexture2D>(mWorld.Engine());
		texView->SetInternalResource(texBuffer);

		mDSs->SetView(std::to_string(0), texView);
	}

	void aiva::layer2::RenderSystem::UseRTsDSs()
	{
		Asserts::CheckBool(mRTs, "RT heap is not valid");
		Asserts::CheckBool(mDSs, "DS heap is not valid");

		auto setRenderTarget = GcaSetRenderTarget{};
		setRenderTarget.RtHeap = mRTs;
		setRenderTarget.DsHeap = mDSs;

		mWorld.Engine().GraphicExecutor().ExecuteCommand(setRenderTarget);
	}

	void aiva::layer2::RenderSystem::ClearRTs()
	{
		Asserts::CheckBool(mRTs, "RT heap is not valid");

		for (std::size_t i = {}; i < std::size_t{ NUM_DEFFERED_BUFFERS }; i++)
		{
			auto clearRenderTarget = GcaClearRenderTarget();
			clearRenderTarget.Heap = mRTs;
			clearRenderTarget.View = std::to_string(i);
			clearRenderTarget.Color = glm::vec4{ 0.0f };

			mWorld.Engine().GraphicExecutor().ExecuteCommand(clearRenderTarget);
		}
	}

	void aiva::layer2::RenderSystem::ClearDSs()
	{
		Asserts::CheckBool(mDSs, "DS heap is not valid");

		auto clearDepthStencil = GcaClearDepthStencil{};
		clearDepthStencil.Heap = mDSs;
		clearDepthStencil.View = std::to_string(0);
		clearDepthStencil.Depth = 1.0f;

		mWorld.Engine().GraphicExecutor().ExecuteCommand(clearDepthStencil);
	}

	void aiva::layer2::RenderSystem::UseViewports()
	{
		auto setViewports = GcaSetViewports{};
		setViewports.Rect = mWorld.Engine().GraphicHardware().ScreenViewRect();

		mWorld.Engine().GraphicExecutor().ExecuteCommand(setViewports);
	}

	void aiva::layer2::RenderSystem::UseScissorRects()
	{
		auto setScissorRects = GcaSetScissorRects{};
		setScissorRects.Rect = mWorld.Engine().GraphicHardware().ScreenViewRect();

		mWorld.Engine().GraphicExecutor().ExecuteCommand(setScissorRects);
	}

	void aiva::layer2::RenderSystem::DrawMeshRenderer(ScCamera const& const camera, ScMeshRenderer const& const meshRenderer)
	{
		auto drawMesh = GcaDrawMesh{};
		drawMesh.Material = SetupCameraProperties(camera, meshRenderer);
		drawMesh.MeshTopology = EPrimitiveTopology::TriangleList;
		drawMesh.MeshIndicesKey = MaterialConstants::AIVA_BUFFER_MESH_INDICES;

		mWorld.Engine().GraphicExecutor().ExecuteCommand(drawMesh);
	}

	RoMaterialGraphicTypeShared aiva::layer2::RenderSystem::SetupCameraProperties(ScCamera const& const camera, ScMeshRenderer const& const meshRenderer)
	{
		auto const sharedMaterial = meshRenderer.Material();
		Asserts::CheckBool(sharedMaterial, "Shared material is not valid");

		auto const instancedMaterial = sharedMaterial->Copy();
		Asserts::CheckBool(instancedMaterial, "Instanced material is not valid");

		auto const constantHeap = instancedMaterial->ResourceDescriptor().ResourceTable().GetOrAddResourceHeap(EDescriptorHeapType::CbvSrvUav);
		Asserts::CheckBool(constantHeap, "Constant heap is not valid");

		auto const constantView = constantHeap->GetOrAddView<GrvCbvToBuffer>(MaterialConstants::AIVA_BUFFER_CONSTANTS_PER_OBJECT);
		Asserts::CheckBool(constantView, "Constant view is not valid");

		{ // MVP
			auto const viewRect = mWorld.Engine().GraphicHardware().ScreenViewRect();
			auto const viewAspect = (viewRect.z - viewRect.x) / (viewRect.w - viewRect.y);

			auto const model = meshRenderer.Actor().WorldTransform();
			constantView->Struct().FieldValue(MaterialConstants::AIVA_CONSTANT_M, model);

			auto const view = camera.Actor().WorldView();
			constantView->Struct().FieldValue(MaterialConstants::AIVA_CONSTANT_V, view);

			auto const projection = glm::perspective(camera.FovY(), viewAspect, camera.ZNear(), camera.ZFar());
			constantView->Struct().FieldValue(MaterialConstants::AIVA_CONSTANT_P, projection);

			auto const mvp = projection * view * model;
			constantView->Struct().FieldValue(MaterialConstants::AIVA_CONSTANT_MVP, mvp);
		}

		return instancedMaterial;
	}

	void aiva::layer2::RenderSystem::PresentST()
	{
		auto present = GcaPresent{};
		mWorld.Engine().GraphicExecutor().ExecuteCommand(present);
	}

	void aiva::layer2::RenderSystem::ShutDSs()
	{
		Asserts::CheckBool(mRTs, "DS heap is not valid");
		mDSs = {};
	}

	void aiva::layer2::RenderSystem::ShutRTs()
	{
		Asserts::CheckBool(mRTs, "RT heap is not valid");
		mRTs = {};
	}

}
