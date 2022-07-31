#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer2
{
	struct RenderSystem;
	struct SceneSystem;
}

namespace aiva::layer2
{
	struct World final : private boost::noncopyable, public std::enable_shared_from_this<World>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<World> Create(Args&&... args);

	public:
		~World();

		void Run();

	private:
		World();

	// ----------------------------------------------------

	private:
		void InitializeEngine();

		void TerminateEngine();

	private:
		void OnEngineStart();

		void OnEngineFinish();

	private:
		std::unique_ptr<aiva::layer1::Engine> mEngine{};

	// ----------------------------------------------------
	// Systems

	public:
		SceneSystem& SceneSystem() const;

		RenderSystem& RenderSystem() const;

	private:
		void InitializeSystems();

		void TerminateSystems();

	private:
		std::shared_ptr<aiva::layer2::SceneSystem> mSceneSystem{};

		std::shared_ptr<aiva::layer2::RenderSystem> mRenderSystem{};

	// ----------------------------------------------------
	// Render

	public:
		void InitializeRender();

		void TickRender();

		void TerminateRender();
	};
}

// --------------------------------------------------------

template<typename... Args>
std::shared_ptr<aiva::layer2::World> aiva::layer2::World::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer2::World>(new aiva::layer2::World{ std::forward<Args>(args)... });
}
