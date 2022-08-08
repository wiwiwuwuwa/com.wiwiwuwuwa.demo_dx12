#include <pch.h>
#include <aiva/layer2/sc_camera.h>

#include <aiva/layer2/render_system.h>
#include <aiva/layer2/scene_actor.h>
#include <aiva/layer2/world.h>

aiva::layer2::ScCamera::ScCamera(SceneActor& actor) : SceneComponent{ actor }
{
	InitializeRenderEvents();
}

aiva::layer2::ScCamera::~ScCamera()
{
	TerminateRenderEvents();
}

float aiva::layer2::ScCamera::FovY() const
{
	return mFovY;
}

aiva::layer2::ScCamera& aiva::layer2::ScCamera::FovY(float const fovy)
{
	mFovY = fovy;
	return *this;
}

float aiva::layer2::ScCamera::ZNear() const
{
	return mZNear;
}

aiva::layer2::ScCamera& aiva::layer2::ScCamera::ZNear(float const znear)
{
	mZNear = znear;
	return *this;
}

float aiva::layer2::ScCamera::ZFar() const
{
	return mZFar;
}

aiva::layer2::ScCamera& aiva::layer2::ScCamera::ZFar(float const zfar)
{
	mZFar = zfar;
	return *this;
}

void aiva::layer2::ScCamera::InitializeRenderEvents()
{
	Actor().World().RenderSystem().OnPopulateCameras().connect(boost::bind(&ScCamera::OnRenderSystemPopulateCameras, this));
}

void aiva::layer2::ScCamera::TerminateRenderEvents()
{
	Actor().World().RenderSystem().OnPopulateCameras().disconnect(boost::bind(&ScCamera::OnRenderSystemPopulateCameras, this));
}

std::shared_ptr<aiva::layer2::ScCamera> aiva::layer2::ScCamera::OnRenderSystemPopulateCameras()
{
	return std::dynamic_pointer_cast<ScCamera>(shared_from_this());
}

