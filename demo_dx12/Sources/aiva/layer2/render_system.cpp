#include <pch.h>
#include <aiva/layer2/render_system.h>

#include <aiva/layer1/engine.h>
#include <aiva/layer1/graphic_pipeline.h>
#include <aiva/layer2/world.h>
#include <aiva/utils/asserts.h>

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
}

void aiva::layer2::RenderSystem::TerminateRenderer()
{
	mWorld.Engine().GraphicPipeline().OnPopulateCommands().disconnect(boost::bind(&RenderSystem::ExecuteRenderer, this));
}

void aiva::layer2::RenderSystem::ExecuteRenderer()
{
	auto const cameras = OnPopulateCameras()();
	for (auto const& const camera : cameras)
	{
		aiva::utils::Asserts::CheckBool(camera, "Camera is not valid");

		auto const meshRenderers = OnPopulateMeshRenderers()();
		for (auto const& const meshRenderer : meshRenderers)
		{
			aiva::utils::Asserts::CheckBool(meshRenderer, "Mesh renderer is not valid");
		}
	}
}
