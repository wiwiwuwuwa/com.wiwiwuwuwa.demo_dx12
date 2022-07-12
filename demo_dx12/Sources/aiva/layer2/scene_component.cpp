#include <pch.h>
#include <aiva/layer2/scene_component.h>

#include <aiva/layer2/scene_actor.h>

aiva::layer2::SceneComponent::~SceneComponent()
{

}

aiva::layer2::SceneComponent::SceneComponent(std::weak_ptr<aiva::layer2::SceneActor> const& actor) : mActor{ actor }
{

}
