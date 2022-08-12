#include <pch.h>
#include <aiva/layer2/scene_component.h>

#include <aiva/layer2/scene_actor.h>
#include <aiva/utils/asserts.h>

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

	void SceneComponent::ParseFromJson(nlohmann::json const&)
	{
		Asserts::CheckBool(false, "SceneComponent::ParseFromJson is not implemented");
	}
}
