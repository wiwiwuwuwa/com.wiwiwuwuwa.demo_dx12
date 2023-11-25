#pragma once
#include <pch.h>

#include <aiva/layer2/scene_component.h>

namespace aiva::layer1
{
	struct RoMaterialGraphic;
}

namespace aiva::layer2
{
	struct SceneActor;
}

namespace aiva::layer2
{
	struct ScMeshRenderer : public SceneComponent
	{
	private:
		friend SceneActor;

	// ----------------------------------------------------
	// Main

	private:
		ScMeshRenderer(SceneActor& actor);

	public:
		~ScMeshRenderer();

	// ----------------------------------------------------
	// Material Info

	public:
		std::shared_ptr<aiva::layer1::RoMaterialGraphic> Material() const;

		ScMeshRenderer& Material(std::shared_ptr<aiva::layer1::RoMaterialGraphic> const& material);

	private:
		std::shared_ptr<aiva::layer1::RoMaterialGraphic> mMaterial{};

	// ----------------------------------------------------
	// Render Events

	private:
		void InitializeRenderEvents();

		void TerminateRenderEvents();

	private:
		std::shared_ptr<ScMeshRenderer> OnRenderSystemPopulateMeshRenderers();
	};
}
