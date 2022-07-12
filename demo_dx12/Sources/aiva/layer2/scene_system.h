#pragma once
#include <pch.h>

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

		~SceneSystem();

	private:
		SceneSystem(std::weak_ptr<aiva::layer2::World> const& world);

	private:
		std::weak_ptr<aiva::layer2::World> mWorld{};

	// ----------------------------------------------------
	// Hierarchy

	public:
		aiva::layer2::SceneActor& CreateActor();

	private:
		std::vector<std::shared_ptr<aiva::layer2::SceneActor>> mActors{};
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer2::SceneSystem> aiva::layer2::SceneSystem::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer2::SceneSystem>(new aiva::layer2::SceneSystem{ std::forward<Args>(args)... });
}
