#include <pch.h>
#include <aiva/layer2/render_system.h>

#include <aiva/layer1/a_graphic_resource_view.h>
#include <aiva/layer1/e_descriptor_heap_type.h>
#include <aiva/layer1/e_primitive_topology.h>
#include <aiva/layer1/e_resource_view_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gca_clear_depth_stencil.h>
#include <aiva/layer1/gca_clear_render_target.h>
#include <aiva/layer1/gca_draw_mesh.h>
#include <aiva/layer1/gca_present.h>
#include <aiva/layer1/gca_set_render_target.h>
#include <aiva/layer1/gca_set_scissor_rects.h>
#include <aiva/layer1/gca_set_viewports.h>
#include <aiva/layer1/gr_texture_2d.h>
#include <aiva/layer1/graphic_executor.h>
#include <aiva/layer1/graphic_hardware.h>
#include <aiva/layer1/graphic_pipeline.h>
#include <aiva/layer1/grv_cbv_to_buffer.h>
#include <aiva/layer1/grv_dsv_to_texture_2d.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/layer1/res_view_desc.h>
#include <aiva/layer1/res_view_desc_utils.h>
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

	RenderSystem::RenderSystem(WorldType const& world) : AObject{}, mWorld{ world }
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
	}

	void RenderSystem::TerminateRenderer()
	{
		mWorld.Engine().GraphicPipeline().OnPopulateCommands().disconnect(boost::bind(&RenderSystem::ExecuteRenderer, this));
	}

	void RenderSystem::ExecuteRenderer()
	{
		auto const screenSize = mWorld.Engine().GraphicHardware().ScreenSize();
		auto const screenRect = glm::vec4{ 0.0f, 0.0f, screenSize.x, screenSize.y };
		auto const defferedBuffer = CreateDefferedBuffer(screenSize);

		ClearRenderTarget(defferedBuffer);
		SetRenderTarget(defferedBuffer);
		SetDrawArea(screenRect);
		DrawModels();

		PresentFrame();
	}

	void RenderSystem::SetDrawArea(glm::vec4 const rect) const
	{
		auto setScissorRects = GcaSetScissorRects{};
		setScissorRects.Rect = rect;

		mWorld.Engine().GraphicExecutor().ExecuteCommand(setScissorRects);

		auto setViewports = GcaSetViewports{};
		setViewports.Rect = rect;

		mWorld.Engine().GraphicExecutor().ExecuteCommand(setViewports);
	}

	void RenderSystem::DrawModels()
	{
		DrawModels(OnPopulateCameras()(), OnPopulateMeshRenderers()());
	}

	void RenderSystem::DrawModels(std::vector<ScCameraTypeShared> const& cameras, std::vector<ScMeshRendererTypeShared> const& meshRenderers) const
	{
		for (auto const& camera : cameras)
		{
			Asserts::CheckBool(camera, "Camera is not valid");
			DrawModels(camera, meshRenderers);
		}
	}

	void RenderSystem::DrawModels(ScCameraTypeShared const& camera, std::vector<ScMeshRendererTypeShared> const& meshRenderers) const
	{
		for (auto const& meshRenderer : meshRenderers)
		{
			Asserts::CheckBool(meshRenderer, "Mesh renderer is not valid");
			DrawModel(camera, meshRenderer);
		}
	}

	void RenderSystem::DrawModel(ScCameraTypeShared const& camera, ScMeshRendererTypeShared const& meshRenderer) const
	{
		Asserts::CheckBool(camera, "Camera is not valid");
		Asserts::CheckBool(meshRenderer, "Mesh renderer is not valid");

		auto const sharedMaterial = meshRenderer->Material();
		Asserts::CheckBool(sharedMaterial, "Shader material is not valid");

		auto const instancedMaterial = sharedMaterial->Copy();
		Asserts::CheckBool(instancedMaterial, "Instanced material is not valid");

		auto const constantHeap = instancedMaterial->ResourceDescriptor().ResourceTable().GetOrAddResourceHeap(EDescriptorHeapType::CbvSrvUav);
		Asserts::CheckBool(constantHeap, "Constant heap is not valid");

		auto const constantView = NewObject<GrvCbvToBuffer>(mWorld.Engine());
		Asserts::CheckBool(constantView, "Constant view is not valid");

		{ // MVP
			auto const screenSize = glm::vec2{ mWorld.Engine().GraphicHardware().ScreenSize() };
			auto const screenAspect = screenSize.x / screenSize.y;

			auto const model = meshRenderer->Actor().WorldTransform();
			constantView->Struct().FieldValue(MaterialConstants::AIVA_CONSTANT_M, model);

			auto const view = camera->Actor().WorldView();
			constantView->Struct().FieldValue(MaterialConstants::AIVA_CONSTANT_V, view);

			auto const projection = glm::perspective(camera->FovY(), screenAspect, camera->ZNear(), camera->ZFar());
			constantView->Struct().FieldValue(MaterialConstants::AIVA_CONSTANT_P, projection);

			auto const mvp = projection * view * model;
			constantView->Struct().FieldValue(MaterialConstants::AIVA_CONSTANT_MVP, mvp);
		}

		constantView->GetOrAddInternalResource();
		constantHeap->SetView(MaterialConstants::AIVA_BUFFER_CONSTANTS_PER_OBJECT, constantView);

		DrawModel(instancedMaterial);
	}

	void RenderSystem::DrawModel(RoMaterialGraphicTypeShared const& sharedMaterial) const
	{
		Asserts::CheckBool(sharedMaterial, "Shared material is not valid");

		auto const instancedMaterial = sharedMaterial->Copy();
		Asserts::CheckBool(instancedMaterial, "Instanced material is not valid");

		auto drawMesh = GcaDrawMesh{};
		drawMesh.Material = instancedMaterial;
		drawMesh.MeshTopology = EPrimitiveTopology::TriangleList;
		drawMesh.MeshIndicesKey = MaterialConstants::AIVA_BUFFER_MESH_INDICES;

		mWorld.Engine().GraphicExecutor().ExecuteCommand(drawMesh);
	}

	void RenderSystem::PresentFrame()
	{
		auto present = GcaPresent{};
		mWorld.Engine().GraphicExecutor().ExecuteCommand(present);
	}

	DefferedBufferType RenderSystem::CreateDefferedBuffer(glm::u64vec2 const size) const
	{
		static constexpr const std::size_t NUM_DEFFERED_RENDER_TARGETS = 4;
		static constexpr const std::size_t NUM_DEFFERED_DEPTH_STENCILS = 1;

		auto defferedBuffer = DefferedBufferType{};

		for (std::size_t i = {}; i < NUM_DEFFERED_RENDER_TARGETS; i++)
		{
			auto rtDesc = CreateRenderTarget(EResourceBufferFormat::R32G32B32A32_FLOAT, size);
			Asserts::CheckBool(ResViewDescUtils::IsValid(rtDesc), "RT desc is not valid");

			defferedBuffer.RTs.emplace_back(rtDesc);
		}

		for (std::size_t i = {}; i < NUM_DEFFERED_DEPTH_STENCILS; i++)
		{
			auto dsDesc = CreateRenderTarget(EResourceBufferFormat::D32_FLOAT, size);
			Asserts::CheckBool(ResViewDescUtils::IsValid(dsDesc), "DS desc is not valid");

			defferedBuffer.DSs.emplace_back(dsDesc);
		}

		return defferedBuffer;
	}

	void RenderSystem::SetRenderTarget(DefferedBufferType const& defferedBuffer) const
	{
		SetRenderTarget(defferedBuffer.RTs, defferedBuffer.DSs);
	}

	void RenderSystem::ClearRenderTarget(DefferedBufferType const& defferedBuffer) const
	{
		for (auto const& rtDesc : defferedBuffer.RTs)
		{
			Asserts::CheckBool(ResViewDescUtils::IsValid(rtDesc), "RT desc is not valid");
			ClearRenderTarget(rtDesc);
		}

		for (auto const& dsDesc : defferedBuffer.DSs)
		{
			Asserts::CheckBool(ResViewDescUtils::IsValid(dsDesc), "DS desc is not valid");
			ClearRenderTarget(dsDesc);
		}
	}

	ResViewDescType RenderSystem::CreateRenderTarget(EResourceBufferFormat const format, glm::u64vec2 const size) const
	{
		Asserts::CheckBool(size.x > 0 && size.y > 0, "Size is not valid");

		static std::uint64_t uniqueID{};
		auto const name = std::to_string(++uniqueID);

		if (!SupportsDepthStencil(format))
		{
			auto const rtHeap = NewObject<ResourceViewHeapType>(mWorld.Engine(), EDescriptorHeapType::Rtv);
			Asserts::CheckBool(rtHeap, "RT heap is not valid");

			auto const rtView = rtHeap->GetOrAddView<GrvRtvToTexture2DType>(name);
			Asserts::CheckBool(rtView, "RT view is not valid");

			auto const rtTexture = std::dynamic_pointer_cast<GrTexture2DType>(rtView->GetOrAddInternalResource());
			Asserts::CheckBool(rtTexture, "RT texture is not valid");

			rtTexture->Format(format);
			rtTexture->Width(size.x);
			rtTexture->Height(size.y);
			rtTexture->SupportRenderTarget(true);
			rtTexture->SupportUnorderedAccess(true);

			return ResViewDescType{ rtHeap, name };
		}
		else
		{
			auto const dsHeap = NewObject<ResourceViewHeapType>(mWorld.Engine(), EDescriptorHeapType::Dsv);
			Asserts::CheckBool(dsHeap, "DS heap is not valid");

			auto const dsView = dsHeap->GetOrAddView<GrvDsvToTexture2DType>(name);
			Asserts::CheckBool(dsView, "DS view is not valid");

			auto const dsTexture = std::dynamic_pointer_cast<GrTexture2DType>(dsView->GetOrAddInternalResource());
			Asserts::CheckBool(dsTexture, "DS texture is not valid");

			dsTexture->Format(format);
			dsTexture->Width(size.x);
			dsTexture->Height(size.y);
			dsTexture->SupportDepthStencil(true);

			return ResViewDescType{ dsHeap, name };
		}
	}

	void RenderSystem::SetRenderTarget(std::vector<ResViewDescType> const& RTs, std::vector<ResViewDescType> const& DSs /*= {}*/) const
	{
		auto setRenderTarget = GcaSetRenderTarget{};
		setRenderTarget.RTs = RTs;
		setRenderTarget.DSs = DSs;

		mWorld.Engine().GraphicExecutor().ExecuteCommand(setRenderTarget);
	}

	void RenderSystem::ClearRenderTarget(ResViewDescType const& rtDesc) const
	{
		Asserts::CheckBool(ResViewDescUtils::IsValid(rtDesc), "RT desc is not valid");

		auto const rtView = ResViewDescUtils::GetView(rtDesc);
		Asserts::CheckBool(rtView, "RT view is not valid");

		switch (rtView->ViewType())
		{
		case EResourceViewType::Rtv:
		{
			auto clearRenderTarget = GcaClearRenderTarget{};
			clearRenderTarget.RT = rtDesc;
			clearRenderTarget.Color = {};

			mWorld.Engine().GraphicExecutor().ExecuteCommand(clearRenderTarget);
			break;
		}

		case EResourceViewType::Dsv:
		{
			auto clearDepthStencil = GcaClearDepthStencil{};
			clearDepthStencil.DS = rtDesc;
			clearDepthStencil.Depth = 1.0f;

			mWorld.Engine().GraphicExecutor().ExecuteCommand(clearDepthStencil);
			break;
		}

		default:
		{
			Asserts::CheckBool(false, "View type is not valid");
			break;
		}
		}
	}
}

//	void RenderSystem::ExecuteRenderer()
//	{
//		UseRTsDSs();
//		ClearRTs();
//		ClearDSs();
//		UseViewports();
//		UseScissorRects();
//
//		auto const cameras = OnPopulateCameras()();
//		auto const meshRenderers = OnPopulateMeshRenderers()();
//
//		for (auto const& const camera : cameras)
//		{
//			Asserts::CheckBool(camera, "Camera is not valid");
//			for (auto const& const meshRenderer : meshRenderers)
//			{
//				Asserts::CheckBool(meshRenderer, "Mesh renderer is not valid");
//				DrawMeshRenderer(*camera, *meshRenderer);
//			}
//		}
//
//		PresentST();
//	}
//
//	void RenderSystem::UseViewports()
//	{
//		auto setViewports = GcaSetViewports{};
//		setViewports.Rect = mWorld.Engine().GraphicHardware().ScreenViewRect();
//
//		mWorld.Engine().GraphicExecutor().ExecuteCommand(setViewports);
//	}
//
//	void RenderSystem::UseScissorRects()
//	{
//		auto setScissorRects = GcaSetScissorRects{};
//		setScissorRects.Rect = mWorld.Engine().GraphicHardware().ScreenViewRect();
//
//		mWorld.Engine().GraphicExecutor().ExecuteCommand(setScissorRects);
//	}
