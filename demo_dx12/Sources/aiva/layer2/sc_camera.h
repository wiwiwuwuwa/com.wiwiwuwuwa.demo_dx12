#pragma once
#include <pch.h>

#include <aiva/layer2/scene_component.h>

namespace aiva::layer2
{
	struct SceneActor;
	struct World;
}

namespace aiva::layer2
{
	struct ScCamera : public SceneComponent
	{
	private:
		friend SceneActor;

	// ----------------------------------------------------
	// Main

	private:
		ScCamera(SceneActor& actor);

	public:
		~ScCamera();

	// ----------------------------------------------------
	// Camera Info

	public:
		float FovY() const;

		ScCamera& FovY(float const fovy);

		float ZNear() const;

		ScCamera& ZNear(float const znear);

		float ZFar() const;

		ScCamera& ZFar(float const zfar);

	private:
		float mFovY = 45.0f;

		float mZNear = 0.1f;

		float mZFar = 256.0f;

	// ----------------------------------------------------
	// Camera Consts

	public:
		glm::mat4 MatrixMVP() const;

	// ----------------------------------------------------
	// Render Events

	private:
		void InitializeRenderEvents();

		void TerminateRenderEvents();

	private:
		std::shared_ptr<ScCamera> OnRenderSystemPopulateCameras();
	};
}
