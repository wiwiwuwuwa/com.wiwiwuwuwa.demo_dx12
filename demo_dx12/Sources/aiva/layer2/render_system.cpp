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
#include <aiva/layer1/grv_dsv_to_texture_2d.h>
#include <aiva/layer1/grv_rtv_to_texture_2d.h>
#include <aiva/layer1/grv_srv_to_buffer.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/layer1/shader_struct.h>
#include <aiva/layer2/sc_camera.h>
#include <aiva/layer2/sc_mesh_renderer.h>
#include <aiva/layer2/world.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/material_constants.h>

aiva::layer2::RenderSystem::RenderSystem(World const& world) : mWorld{ world }
{
	InitializeRenderer();
}

aiva::layer2::RenderSystem::~RenderSystem()
{
	TerminateRenderer();
}

aiva::layer2::RenderSystem::EvPopulateCameras& aiva::layer2::RenderSystem::OnPopulateCameras()
{
	return mOnPopulateCameras;
}

aiva::layer2::RenderSystem::EvPopulateMeshRenderers& aiva::layer2::RenderSystem::OnPopulateMeshRenderers()
{
	return mOnPopulateMeshRenderers;
}

void aiva::layer2::RenderSystem::InitializeRenderer()
{
	mWorld.Engine().GraphicPipeline().OnPopulateCommands().connect(boost::bind(&RenderSystem::ExecuteRenderer, this));
	InitRTs();
	InitDSs();
}

void aiva::layer2::RenderSystem::TerminateRenderer()
{
	ShutDSs();
	ShutRTs();
	mWorld.Engine().GraphicPipeline().OnPopulateCommands().disconnect(boost::bind(&RenderSystem::ExecuteRenderer, this));
}

void aiva::layer2::RenderSystem::ExecuteRenderer()
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
		aiva::utils::Asserts::CheckBool(camera, "Camera is not valid");
		for (auto const& const meshRenderer : meshRenderers)
		{
			aiva::utils::Asserts::CheckBool(meshRenderer);
			DrawMeshRenderer(*camera, *meshRenderer);
		}
	}

	PresentST();
}

void aiva::layer2::RenderSystem::InitRTs()
{
	auto const viewRect = mWorld.Engine().GraphicHardware().ScreenViewRect();

	mRTs = aiva::layer1::ResourceViewHeap::Create(mWorld.Engine(), aiva::layer1::EDescriptorHeapType::Rtv);
	aiva::utils::Asserts::CheckBool(mRTs, "RT heap is not valid");

	for (std::size_t i = {}; i < std::size_t{ NUM_DEFFERED_BUFFERS }; i++)
	{
		auto texBufferDesc = aiva::layer1::GrTexture2DDesc{};
		texBufferDesc.BufferFormat = aiva::layer1::EResourceBufferFormat::R32G32B32A32_FLOAT;
		texBufferDesc.Width = viewRect.z;
		texBufferDesc.Height = viewRect.w;
		texBufferDesc.SupportRenderTarget = true;
		texBufferDesc.SupportUnorderedAccess = true;

		auto texBufferResource = aiva::layer1::GrTexture2D::Create(mWorld.Engine(), texBufferDesc);
		aiva::utils::Asserts::CheckBool(texBufferResource, "Tex buffer is not valid");

		auto texViewDesc = aiva::layer1::GrvRtvToTexture2DDesc{};
		texViewDesc.Resource = texBufferResource;

		auto texViewResource = aiva::layer1::GrvRtvToTexture2D::Create(mWorld.Engine(), texViewDesc);
		aiva::utils::Asserts::CheckBool(texViewResource, "Tex view is not valid");

		mRTs->ResourceView(std::to_string(i), texViewResource);
	}
}

void aiva::layer2::RenderSystem::InitDSs()
{
	auto const viewRect = mWorld.Engine().GraphicHardware().ScreenViewRect();

	mDSs = aiva::layer1::ResourceViewHeap::Create(mWorld.Engine(), aiva::layer1::EDescriptorHeapType::Dsv);
	aiva::utils::Asserts::CheckBool(mDSs, "DS heap is not valid");

	auto texBufferDesc = aiva::layer1::GrTexture2DDesc{};
	texBufferDesc.BufferFormat = aiva::layer1::EResourceBufferFormat::D32_FLOAT;
	texBufferDesc.Width = viewRect.z;
	texBufferDesc.Height = viewRect.w;
	texBufferDesc.SupportDepthStencil = true;

	auto texBufferResource = aiva::layer1::GrTexture2D::Create(mWorld.Engine(), texBufferDesc);
	aiva::utils::Asserts::CheckBool(texBufferResource, "Tex buffer is not valid");

	auto texViewDesc = aiva::layer1::GrvDsvToTexture2DDesc{};
	texViewDesc.Resource = texBufferResource;

	auto texViewResource = aiva::layer1::GrvDsvToTexture2D::Create(mWorld.Engine(), texViewDesc);
	aiva::utils::Asserts::CheckBool(texViewResource, "Tex view is not valid");

	mDSs->ResourceView(std::to_string(0), texViewResource);
}

void aiva::layer2::RenderSystem::UseRTsDSs()
{
	aiva::utils::Asserts::CheckBool(mRTs, "RT heap is not valid");
	aiva::utils::Asserts::CheckBool(mDSs, "DS heap is not valid");

	auto setRenderTarget = aiva::layer1::GcaSetRenderTarget{};
	setRenderTarget.RtHeap = mRTs;
	setRenderTarget.DsHeap = mDSs;

	mWorld.Engine().GraphicExecutor().ExecuteCommand(setRenderTarget);
}

void aiva::layer2::RenderSystem::ClearRTs()
{
	aiva::utils::Asserts::CheckBool(mRTs, "RT heap is not valid");

	for (std::size_t i = {}; i < std::size_t{ NUM_DEFFERED_BUFFERS }; i++)
	{
		auto clearRenderTarget = aiva::layer1::GcaClearRenderTarget();
		clearRenderTarget.Heap = mRTs;
		clearRenderTarget.View = std::to_string(i);
		clearRenderTarget.Color = glm::vec4{ 0.0f };

		mWorld.Engine().GraphicExecutor().ExecuteCommand(clearRenderTarget);
	}
}

void aiva::layer2::RenderSystem::ClearDSs()
{
	aiva::utils::Asserts::CheckBool(mDSs, "DS heap is not valid");

	auto clearDepthStencil = aiva::layer1::GcaClearDepthStencil{};
	clearDepthStencil.Heap = mDSs;
	clearDepthStencil.View = std::to_string(0);
	clearDepthStencil.Depth = 1.0f;

	mWorld.Engine().GraphicExecutor().ExecuteCommand(clearDepthStencil);
}

void aiva::layer2::RenderSystem::UseViewports()
{
	auto setViewports = aiva::layer1::GcaSetViewports{};
	setViewports.Rect = mWorld.Engine().GraphicHardware().ScreenViewRect();

	mWorld.Engine().GraphicExecutor().ExecuteCommand(setViewports);
}

void aiva::layer2::RenderSystem::UseScissorRects()
{
	auto setScissorRects = aiva::layer1::GcaSetScissorRects{};
	setScissorRects.Rect = mWorld.Engine().GraphicHardware().ScreenViewRect();

	mWorld.Engine().GraphicExecutor().ExecuteCommand(setScissorRects);
}

void aiva::layer2::RenderSystem::DrawMeshRenderer(ScCamera const& const camera, ScMeshRenderer const& const meshRenderer)
{
	auto const sharedMaterial = meshRenderer.Material();
	aiva::utils::Asserts::CheckBool(sharedMaterial, "Shared material is not valid");

	auto const material = sharedMaterial->Copy();
	aiva::utils::Asserts::CheckBool(material, "Material is not valid");

	SetupCameraProperties(camera, *material);

	auto drawMesh = aiva::layer1::GcaDrawMesh{};
	drawMesh.Material = material;
	drawMesh.MeshTopology = aiva::layer1::EPrimitiveTopology::TriangleList;
	drawMesh.MeshIndicesKey = aiva::utils::MaterialConstants::AIVA_BUFFER_INDICES;

	mWorld.Engine().GraphicExecutor().ExecuteCommand(drawMesh);
}

void aiva::layer2::RenderSystem::SetupCameraProperties(ScCamera const& const camera, aiva::layer1::RoMaterialGraphic const& const material)
{
	auto const constantStruct = aiva::layer1::ShaderStruct::Create();

	auto const constantMVP = camera.MatrixMVP();
	constantStruct->SetValue(aiva::utils::MaterialConstants::AIVA_CONSTANT_MVP, &constantMVP);

	auto const constantHeap = material.ResourceDescriptor().ResourceTable().GetOrAddResourceHeap(aiva::layer1::EDescriptorHeapType::CbvSrvUav);
	aiva::utils::Asserts::CheckBool(constantHeap);

	auto constantView = constantHeap->ResourceView<aiva::layer1::GrvSrvToBuffer>(aiva::utils::MaterialConstants::AIVA_BUFFER_CONSTANT);
	if (!constantView)
	{
		constantView = aiva::layer1::GrvSrvToBuffer::Create(mWorld.Engine());
		constantHeap->ResourceView(aiva::utils::MaterialConstants::AIVA_BUFFER_CONSTANT, constantView);
	}

	aiva::utils::Asserts::CheckBool(constantView, "Constant view is not valid");

	auto constantViewDesc = constantView->Desc();
	if (!constantViewDesc)
	{
		auto constantBufferDesc = aiva::layer1::GrBufferDesc{};
		constantBufferDesc.MemoryType = aiva::layer1::EResourceMemoryType::CpuToGpu;

		auto constantBuffer = aiva::layer1::GrBuffer::Create(mWorld.Engine(), constantBufferDesc);
		aiva::utils::Asserts::CheckBool(constantBuffer, "Constant buffer is not valid");

		constantViewDesc = aiva::layer1::GrvSrvToBufferDesc{};
		constantViewDesc->Resource = constantBuffer;
	}

	aiva::utils::Asserts::CheckBool(constantViewDesc, "Constant view desc is not valid");

	constantViewDesc->Struct = constantStruct;
	constantView->Desc(constantViewDesc);
	constantView->Buffer().Add(constantStruct);
}

void aiva::layer2::RenderSystem::PresentST()
{
	auto present = aiva::layer1::GcaPresent{};
	mWorld.Engine().GraphicExecutor().ExecuteCommand(present);
}

void aiva::layer2::RenderSystem::ShutDSs()
{
	aiva::utils::Asserts::CheckBool(mRTs, "DS heap is not valid");
	mDSs = {};
}

void aiva::layer2::RenderSystem::ShutRTs()
{
	aiva::utils::Asserts::CheckBool(mRTs, "RT heap is not valid");
	mRTs = {};
}
