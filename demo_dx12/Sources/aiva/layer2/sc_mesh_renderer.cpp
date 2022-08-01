#include <pch.h>
#include <aiva/layer2/sc_mesh_renderer.h>

#include <aiva/layer2/render_system.h>
#include <aiva/layer2/scene_actor.h>
#include <aiva/layer2/world.h>

aiva::layer2::ScMeshRenderer::ScMeshRenderer(SceneActor& actor) : SceneComponent{ actor }
{
	InitializeRenderEvents();
}

aiva::layer2::ScMeshRenderer::~ScMeshRenderer()
{
	TerminateRenderEvents();
}

std::shared_ptr<aiva::layer1::RoMaterialGraphic> aiva::layer2::ScMeshRenderer::Material() const
{
	return mMaterial;
}

aiva::layer2::ScMeshRenderer& aiva::layer2::ScMeshRenderer::Material(std::shared_ptr<aiva::layer1::RoMaterialGraphic> const& material)
{
	mMaterial = material;
	return *this;
}

void aiva::layer2::ScMeshRenderer::InitializeRenderEvents()
{
	Actor().World().RenderSystem().OnPopulateMeshRenderers().connect(boost::bind(&ScMeshRenderer::OnRenderSystemPopulateMeshRenderers, this));
}

void aiva::layer2::ScMeshRenderer::TerminateRenderEvents()
{
	Actor().World().RenderSystem().OnPopulateMeshRenderers().disconnect(boost::bind(&ScMeshRenderer::OnRenderSystemPopulateMeshRenderers, this));
}

std::shared_ptr<aiva::layer2::ScMeshRenderer> aiva::layer2::ScMeshRenderer::OnRenderSystemPopulateMeshRenderers()
{
	return std::dynamic_pointer_cast<ScMeshRenderer>(shared_from_this());
}
