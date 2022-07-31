#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct RoSceneGltf;
}

namespace aiva::layer2
{
	struct SceneActor;
	struct World;
}

namespace aiva::layer2
{
	struct SceneSystem final : private boost::noncopyable, public std::enable_shared_from_this<SceneSystem>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<SceneSystem> Create(Args&&... args);

	private:
		SceneSystem(World const& world);

	public:
		~SceneSystem();

	private:
		World const& mWorld;

	// ----------------------------------------------------
	// Hierarchy

	public:
		SceneActor& CreateActor();

	private:
		std::vector<std::shared_ptr<SceneActor>> mActors{};

	// ----------------------------------------------------
	// Scenes

	public:
		void LoadScene(aiva::layer1::RoSceneGltf const& scene);
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer2::SceneSystem> aiva::layer2::SceneSystem::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer2::SceneSystem>(new aiva::layer2::SceneSystem{ std::forward<Args>(args)... });
}
