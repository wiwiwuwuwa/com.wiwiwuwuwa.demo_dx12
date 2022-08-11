#pragma once
#include <pch.h>

#include <aiva/layer1/e_resource_buffer_format.h>
#include <aiva/layer1/res_view_desc.h>
#include <aiva/layer2/sc_camera_fwd.h>
#include <aiva/layer2/sc_mesh_renderer_fwd.h>
#include <aiva/layer2/world_fwd.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/t_signal_aggregator.h>

namespace aiva::layer2
{
	struct RenderSystem final : public aiva::utils::AObject
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		RenderSystem(WorldType const& world);

	public:
		~RenderSystem() override;

	private:
		World const& mWorld;

	// ----------------------------------------------------
	// Events

	public:
		using EvPopulateCameras = boost::signals2::signal<ScCameraTypeShared(), aiva::utils::TSignalAggregator<std::vector<ScCameraTypeShared>>>;

		using EvPopulateMeshRenderers = boost::signals2::signal<std::shared_ptr<ScMeshRenderer>(), aiva::utils::TSignalAggregator<std::vector<ScMeshRendererTypeShared>>>;

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

	// ----------------------------------------------------
	// Render Commands

	private:
		void PresentFrame();

	// ----------------------------------------------------
	// Render Targets

	private:
		aiva::layer1::ResViewDescType CreateRenderTarget(aiva::layer1::EResourceBufferFormat const format, glm::u64vec2 const size) const;

		void SetRenderTarget(std::vector<aiva::layer1::ResViewDescType> const& RTs, std::vector<aiva::layer1::ResViewDescType> const& DSs = {}) const;

		void ClearRenderTarget(aiva::layer1::ResViewDescType const& rtDesc) const;
	};
}
