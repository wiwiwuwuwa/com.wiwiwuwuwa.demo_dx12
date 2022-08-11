#pragma once
#include <pch.h>

#include <aiva/layer2/scene_actor_fwd.h>
#include <aiva/layer2/scene_component_fwd.h>
#include <aiva/utils/a_object.h>

namespace aiva::layer2
{
	struct SceneComponent : public aiva::utils::AObject
	{
	// ----------------------------------------------------
	// Main

	private:
		friend SceneActorType;

	protected:
		SceneComponent(SceneActorType& actor);

	public:
		virtual ~SceneComponent();

	public:
		SceneActorType& Actor() const;

	private:
		SceneActorType& mActor;
	};
}
