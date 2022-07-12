#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct SceneActor;
	struct World;
}

namespace aiva::layer2
{
	struct SceneSystem final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Main

	public:
		SceneSystem(aiva::layer2::World const& world);

		~SceneSystem();

	private:
		aiva::layer2::World const& mWorld;

	// ----------------------------------------------------
	// Hierarchy

	public:
		std::shared_ptr<aiva::layer2::SceneActor> SceneActor() const;

	private:
		void InitializeHierarchy();

		void TerminateHierarchy();

	private:
		std::shared_ptr<aiva::layer2::SceneActor> mSceneActor{};
	};
}