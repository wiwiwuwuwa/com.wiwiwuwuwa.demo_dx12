#pragma once
#include <pch.h>

#include <aiva/utils/t_signal_aggregator.h>

namespace aiva::layer2
{
	struct ScCamera;
	struct ScMeshRenderer;
	struct World;
}

namespace aiva::layer2
{
	struct RenderSystem final : private boost::noncopyable, public std::enable_shared_from_this<RenderSystem>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... Args>
		static std::shared_ptr<RenderSystem> Create(Args&&... args);

	private:
		RenderSystem(World const& world);

	public:
		~RenderSystem();

	private:
		World const& mWorld;

	// ----------------------------------------------------
	// Events

	public:
		using EvPopulateCameras = boost::signals2::signal<std::shared_ptr<ScCamera>(), aiva::utils::TSignalAggregator<std::vector<std::shared_ptr<ScCamera>>>>;

		using EvPopulateMeshRenderers = boost::signals2::signal<std::shared_ptr<ScMeshRenderer>(), aiva::utils::TSignalAggregator<std::vector<std::shared_ptr<ScMeshRenderer>>>>;

	public:
		EvPopulateCameras& OnPopulateCameras();

		EvPopulateMeshRenderers& OnPopulateMeshRenderers();

	private:
		EvPopulateCameras mOnPopulateCameras{};

		EvPopulateMeshRenderers mOnPopulateMeshRenderers{};

	// ----------------------------------------------------
	// Renderer

	private:
		void InitializeRenderer();

		void TerminateRenderer();

	private:
		void ExecuteRenderer();
	};
}

// --------------------------------------------------------

template <typename... Args>
std::shared_ptr<aiva::layer2::RenderSystem> aiva::layer2::RenderSystem::Create(Args&&... args)
{
	return std::shared_ptr<aiva::layer2::RenderSystem>(new aiva::layer2::RenderSystem{ std::forward<Args>(args)... });
}
