#include <pch.h>
#include <aiva/layer2/scene_component.h>

#include <aiva/layer2/scene_actor.h>

aiva::layer2::SceneComponent::SceneComponent(SceneActor& actor) : mActor{ actor }
{

}

aiva::layer2::SceneComponent::~SceneComponent()
{

}

aiva::layer2::SceneActor& aiva::layer2::SceneComponent::Actor() const
{
	return mActor;
}
