#include <pch.h>
#include <aiva/layer2/world.h>

#include <aiva/layer1/e_gpu_descriptor_heap_type.h>
#include <aiva/layer1/engine.h>
#include <aiva/layer1/gca_dispatch.h>
#include <aiva/layer1/gca_do_everything.h>
#include <aiva/layer1/graphic_executor.h>
#include <aiva/layer1/graphic_pipeline.h>
#include <aiva/layer1/resource_system.h>
#include <aiva/layer1/resource_view_heap.h>
#include <aiva/layer1/resource_view_table.h>
#include <aiva/layer1/ro_material_compute.h>
#include <aiva/layer2/scene_system.h>
#include <aiva/utils/asserts.h>

aiva::layer2::World::World()
{
	InitializeEngine();
}

aiva::layer2::World::~World()
{
	TerminateEngine();
}

void aiva::layer2::World::Run()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine->Run();
}

void aiva::layer2::World::InitializeEngine()
{
	mEngine = std::make_unique<aiva::layer1::Engine>();
	aiva::utils::Asserts::CheckBool(mEngine);

	mEngine->OnStart().connect(boost::bind(&aiva::layer2::World::OnEngineStart, this));
	mEngine->OnFinish().connect(boost::bind(&aiva::layer2::World::OnEngineFinish, this));
}

void aiva::layer2::World::TerminateEngine()
{
	aiva::utils::Asserts::CheckBool(mEngine);

	mEngine->OnFinish().disconnect(boost::bind(&aiva::layer2::World::OnEngineFinish, this));
	mEngine->OnStart().disconnect(boost::bind(&aiva::layer2::World::OnEngineStart, this));

	mEngine = {};
}

void aiva::layer2::World::OnEngineStart()
{
	InitializeSystems();
	InitializeRender();
}

void aiva::layer2::World::OnEngineFinish()
{
	TerminateRender();
	TerminateSystems();
}

aiva::layer2::SceneSystem& aiva::layer2::World::SceneSystem() const
{
	aiva::utils::Asserts::CheckBool(mSceneSystem);
	return *mSceneSystem;
}

void aiva::layer2::World::InitializeSystems()
{
	mSceneSystem = aiva::layer2::SceneSystem::Create(weak_from_this());
	aiva::utils::Asserts::CheckBool(mSceneSystem);
}

void aiva::layer2::World::TerminateSystems()
{
	aiva::utils::Asserts::CheckBool(mSceneSystem);
	mSceneSystem = {};
}

#include <aiva/layer1/gr_buffer.h>
#include <aiva/layer1/gca_draw_mesh.h>
#include <aiva/layer1/gca_set_viewports.h>
#include <aiva/layer1/grv_srv_to_buffer.h>
#include <aiva/layer1/material_pipeline_descriptor.h>
#include <aiva/layer1/ro_material_graphic.h>
#include <aiva/layer1/shader_buffer.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/layer1/shader_struct.h>

static std::shared_ptr<aiva::layer1::RoMaterialGraphic> GLOBAL_GRAPHIC_MATERIAL{};

void aiva::layer2::World::InitializeRender()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine->GraphicPipeline().OnPopulateCommands().connect(boost::bind(&aiva::layer2::World::TickRender, this));

	auto const& indexStructReference = aiva::layer1::ShaderStruct::Create();
	{
		auto index = std::uint32_t{};
		indexStructReference->SetValue("m0_Index", &index);
	}

	auto indexBufferDesc = aiva::layer1::GrBufferDesc{};
	indexBufferDesc.MemoryType = aiva::layer1::EGpuResourceMemoryType::CpuToGpu;
	indexBufferDesc.Size = 0;
	indexBufferDesc.SupportShaderAtomics = false;
	indexBufferDesc.SupportUnorderedAccess = false;

	auto const& indexBuffer = aiva::layer1::GrBuffer::Create(*mEngine);
	indexBuffer->Desc(indexBufferDesc);

	auto bufferViewDesc = aiva::layer1::GrvSrvToBufferDesc{};
	bufferViewDesc.Resource = indexBuffer;
	bufferViewDesc.Struct = indexStructReference;

	auto const& bufferView = aiva::layer1::GrvSrvToBuffer::Create(*mEngine);
	bufferView->Desc(bufferViewDesc);

	for (std::uint32_t i = std::uint32_t{ 0 }; i < std::uint32_t{ 3 }; i++)
	{
		auto const& indexStruct = aiva::layer1::ShaderStruct::Create();
		indexStruct->SetValue("m0_Index", &i);

		bufferView->Buffer().Add(indexStruct);
	}

	auto const& resourceHeap = aiva::layer1::ResourceViewHeap::Create(*mEngine);
	resourceHeap->ResourceType(aiva::layer1::EGpuDescriptorHeapType::CbvSrvUav);
	resourceHeap->ResourceView("t0_Indices", bufferView);

	auto const& material = mEngine->ResourceSystem().GetResource<aiva::layer1::RoMaterialGraphic>("resources\\materials\\checker.mat_gs");
	material->PipelineDescriptor().FillMode(aiva::layer1::EGpuFillMode::Solid);
	material->PipelineDescriptor().CullMode(aiva::layer1::EGpuCullMode::None);
	material->PipelineDescriptor().DepthTest(false);
	material->PipelineDescriptor().DepthWrite(false);
	material->PipelineDescriptor().DepthFunc(aiva::layer1::EGpuComparisonFunc::Always);
	material->PipelineDescriptor().RenderTargets({ aiva::layer1::EGpuResourceBufferFormat::R8G8B8A8_UNORM });
	material->PipelineDescriptor().DepthTarget(aiva::layer1::EGpuResourceBufferFormat::D32_FLOAT);
	material->ResourceDescriptor().ResourceTable().ResourceHeap(aiva::layer1::EGpuDescriptorHeapType::CbvSrvUav, resourceHeap);	

	GLOBAL_GRAPHIC_MATERIAL = material;
}

void aiva::layer2::World::TickRender()
{
	aiva::utils::Asserts::CheckBool(mEngine);

	auto& const gcaSetViewports = aiva::layer1::GcaSetViewports{};

	mEngine->GraphicExecutor().ExecuteCommand(gcaSetViewports);

	auto &const gcaDrawMesh = aiva::layer1::GcaDrawMesh{};
	gcaDrawMesh.Material = GLOBAL_GRAPHIC_MATERIAL;
	gcaDrawMesh.MeshTopology = aiva::layer1::EGpuPrimitiveTopology::TriangleList;
	gcaDrawMesh.MeshIndicesKey = "t0_Indices";

	mEngine->GraphicExecutor().ExecuteCommand(gcaDrawMesh);
}

void aiva::layer2::World::TerminateRender()
{
	aiva::utils::Asserts::CheckBool(mEngine);
	mEngine->GraphicPipeline().OnPopulateCommands().disconnect(boost::bind(&aiva::layer2::World::TickRender, this));
}
