#pragma once
#include <pch.h>

#include <aiva/layer1/e_resource_buffer_format.h>
#include <aiva/layer1/res_view_desc.h>
#include <aiva/layer1/ro_material_graphic_fwd.h>
#include <aiva/layer2/deffered_buffer.h>
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
	// High-Level Drawing Commands

	private:
		void InitDefferedBuffer(DefferedBufferType& defferedBuffer);

		void DrawModelsToDefferedBuffer(DefferedBufferType const& defferedBuffer);

		void InitScreenBuffer();

		void DrawDefferedBufferToScreen(DefferedBufferType const& defferedBuffer);

		void ShutScreenBuffer();

	// ----------------------------------------------------
	// Mid-Level Drawing Commands

	private:
		void BlitQuad(aiva::layer1::ResViewDescType const& mainTexture) const;

	private:
		void InitializeBlitMaterial();

		void TerminateBlitMaterial();

	private:
		aiva::layer1::RoMaterialGraphicTypeShared mBlitMaterial{};

	// --------------------------------

	private:
		void DrawQuad(aiva::layer1::RoMaterialGraphicTypeShared const& sharedMaterial) const;

	private:
		void InitializeQuadModel();

		void TerminateQuadModel();

	private:
		aiva::layer1::RoMaterialGraphicTypeShared mQuadModel{};

	// ----------------------------------------------------
	// Low-Level Drawing Commands

	private:
		void SetDrawArea(glm::vec4 const rect) const;

		void DrawModels();

		void DrawModels(std::vector<ScCameraTypeShared> const& cameras, std::vector<ScMeshRendererTypeShared> const& meshRenderers) const;

		void DrawModels(ScCameraTypeShared const& camera, std::vector<ScMeshRendererTypeShared> const& meshRenderers) const;

		void DrawModel(ScCameraTypeShared const& camera, ScMeshRendererTypeShared const& meshRenderer) const;

		void DrawModel(aiva::layer1::RoMaterialGraphicTypeShared const& sharedMaterial) const;

		void PresentFrame();

	// ----------------------------------------------------
	// Deffered Buffer

	private:
		DefferedBufferType CreateDefferedBuffer(glm::u64vec2 const size) const;

		void SetRenderTarget(DefferedBufferType const& defferedBuffer) const;

		void ClearRenderTarget(DefferedBufferType const& defferedBuffer) const;

	// ----------------------------------------------------
	// Render Targets

	private:
		aiva::layer1::ResViewDescType CreateRenderTarget(aiva::layer1::EResourceBufferFormat const format, glm::u64vec2 const size) const;

		void SetRenderTarget(aiva::layer1::ResViewDescType const& rt = {}, aiva::layer1::ResViewDescType const& ds = {}) const;

		void SetRenderTarget(std::vector<aiva::layer1::ResViewDescType> const& RTs, std::vector<aiva::layer1::ResViewDescType> const& DSs = {}) const;

		void ClearRenderTarget(aiva::layer1::ResViewDescType const& rtDesc) const;
	};
}
