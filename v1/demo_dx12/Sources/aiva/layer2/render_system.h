#pragma once
#include <pch.h>

#include <aiva/layer1/e_resource_buffer_format.h>
#include <aiva/layer1/gr_texture_2d_fwd.h>
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
		static constexpr const auto DEFFERED_BUFFER_SCALE = 1.0f;

		static constexpr const auto DEFFERED_BUFFER_EMISSION_INDEX = 3;

	private:
		void InitializeDefferedPipeline();

		void TerminateDefferedPipeline();

	private:
		void ExecuteDefferedPipeline();

	private:
		void PrepareDefferedBuffer();

		void DrawModelsToDefferedBuffer();

		void PrepareScreenBuffer();

		void DrawDefferedBufferToScreen();

		void PrepareWindowBuffer();

	private:
		void InitializeDefferedScreenSize();

		void InitializeDefferedBufferSize();

		void InitializeDefferedBuffer();

		void InitializeDefferedBlitMaterial();

		void TerminateDefferedBlitMaterial();

		void TerminateDefferedBuffer();

		void TerminateDefferedBufferSize();

		void TerminateDefferedScreenSize();

	private:
		glm::vec2 mDefferedScreenSize{};

		glm::vec2 mDefferedBufferSize{};

		DefferedBufferType mDefferedBuffer{};

		aiva::layer1::RoMaterialGraphicTypeShared mDefferedBlitMaterial{};

	//// ----------------------------------------------------
	//// Mid-Level Drawing Commands

	//private:
	//	void BlitQuad(aiva::layer1::ResViewDescType const& mainTexture, std::size_t const uniqueID) const;

	//private:
	//	void InitializeBlitMaterial();

	//	void TerminateBlitMaterial();

	//private:
	//	aiva::layer1::RoMaterialGraphicTypeShared mBlitMaterial{};

	//// --------------------------------

	//private:
	//	void DrawQuad(aiva::layer1::RoMaterialGraphicTypeShared const& sharedMaterial, std::size_t const uniqueID) const;

	//private:
	//	void InitializeQuadModel();

	//	void TerminateQuadModel();

	//private:
	//	aiva::layer1::RoMaterialGraphicTypeShared mQuadModel{};

	// ----------------------------------------------------
	// Low-Level Drawing Commands

	private:
		void SetDrawArea(glm::vec4 const rect) const;

		void DrawModels();

		void DrawModels(std::vector<ScCameraTypeShared> const& cameras, std::vector<ScMeshRendererTypeShared> const& meshRenderers) const;

		void DrawModels(ScCameraTypeShared const& camera, std::vector<ScMeshRendererTypeShared> const& meshRenderers) const;

		void DrawModel(ScCameraTypeShared const& camera, ScMeshRendererTypeShared const& meshRenderer) const;

		void DrawModel(aiva::layer1::RoMaterialGraphicTypeShared const& instancedMaterial) const;

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

	// ----------------------------------------------------
	// Shared Resources

	private:
		void InitializeSharedResources();

		void TerminateSharedResources();

	// --------------------------------

	private:
		aiva::layer1::RoMaterialGraphicTypeShared const& SharedQuadModel() const;

	private:
		void InitializeSharedQuadModel();

		void TerminateSharedQuadModel();

	private:
		aiva::layer1::RoMaterialGraphicTypeShared mSharedQuadModel{};

	// --------------------------------

	private:
		aiva::layer1::RoMaterialGraphicTypeShared const& SharedBlitMaterial() const;

	private:
		void InitializeSharedBlitMaterial();

		void TerminateSharedBlitMaterial();

	private:
		aiva::layer1::RoMaterialGraphicTypeShared mSharedBlitMaterial{};
	};
}
