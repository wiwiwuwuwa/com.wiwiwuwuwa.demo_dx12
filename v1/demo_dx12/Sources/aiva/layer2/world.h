#pragma once
#include <pch.h>

#include <aiva/layer2/world_fwd.h>
#include <aiva/layer2_ext/render_system_fwd.h>
#include <aiva/layer2_ext/sc_system_fwd.h>

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

	public:
		aiva::layer1::Engine& Engine() const;

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

		aiva::layer2_ext::ScSystem& SceneSystemExt() const;

		RenderSystem& RenderSystem() const;
		
		aiva::layer2_ext::RenderSystem& RenderSystemExt() const;

	private:
		void InitializeSystems();

		void TerminateSystems();

	private:
		std::shared_ptr<aiva::layer2::SceneSystem> mSceneSystem{};

		std::shared_ptr<aiva::layer2_ext::ScSystem> mSceneSystemExt{};

		std::shared_ptr<aiva::layer2::RenderSystem> mRenderSystem{};

		std::shared_ptr<aiva::layer2_ext::RenderSystem> mRenderSystemExt{};
	};
}

// --------------------------------------------------------

template<typename... Args>
std::shared_ptr<aiva::layer2::World> aiva::layer2::World::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer2::World>(new aiva::layer2::World{ std::forward<Args>(args)... });
}
