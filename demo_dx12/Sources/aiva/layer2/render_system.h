#pragma once
#include <pch.h>

#include <aiva/layer1/resource_view_heap_fwd.h>
#include <aiva/layer1/ro_material_graphic_fwd.h>
#include <aiva/layer2/sc_camera_fwd.h>
#include <aiva/layer2/sc_mesh_renderer_fwd.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/t_signal_aggregator.h>

namespace aiva::layer2
{
	struct World;
}

namespace aiva::layer2
{
	struct RenderSystem final : public aiva::utils::AObject
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		RenderSystem(World const& world);

	public:
		~RenderSystem() override;

	private:
		World const& mWorld;

	// ----------------------------------------------------
	// Events

	public:
		using EvPopulateCameras = boost::signals2::signal<std::shared_ptr<ScCamera>(), aiva::utils::TSignalAggregator<std::vector<ScCameraTypeShared>>>;

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
		static constexpr std::size_t NUM_DEFFERED_BUFFERS = 4;

	private:
		void InitializeRenderer();

		void TerminateRenderer();

	private:
		void ExecuteRenderer();

	private:
		void InitRTs();

		void InitDSs();

		void UseRTsDSs();

		void ClearRTs();

		void ClearDSs();

		void UseViewports();

		void UseScissorRects();

		void DrawMeshRenderer(ScCamera const& const camera, ScMeshRenderer const& const meshRenderer);

		aiva::layer1::RoMaterialGraphicTypeShared SetupCameraProperties(ScCamera const& const camera, ScMeshRenderer const& const meshRenderer);

		void PresentST();

		void ShutDSs();

		void ShutRTs();

	private:
		aiva::layer1::ResourceViewHeapTypeShared mRTs{};

		aiva::layer1::ResourceViewHeapTypeShared mDSs{};
	};
}
