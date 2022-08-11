#include <pch.h>
#include <aiva/layer2/scene_component.h>

#include <aiva/layer2/scene_actor.h>

namespace aiva::layer2
{
	using namespace aiva::utils;

	SceneComponent::SceneComponent(SceneActorType& actor) : AObject{}, mActor { actor }
	{

	}

	SceneComponent::~SceneComponent()
	{

	}

	SceneActorType& SceneComponent::Actor() const
	{
		return mActor;
	}
}
