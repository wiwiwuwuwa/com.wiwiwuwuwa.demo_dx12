#pragma once
#include <pch.h>

#include <aiva/layer1_ext/ro_material_gr_fwd.h>
#include <aiva/layer1_ext/ro_mesh_fwd.h>
#include <aiva/layer2/world_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer2_ext
{
	struct RenderSystem final : public aiva::utils_ext::TObject<RenderSystem>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(RenderSystem);

	protected:
		RenderSystem(aiva::layer2::World& world);

	public:
		~RenderSystem() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer2::World, World);

	// ----------------------------------------------------
	// Renderer Fields

	private:
		void InitRendererFields();

		void ShutRendererFields();

	private:
		void RendererFields_When_GraphicPipeline_OnPopulateCommands();

	// ----------------------------------------------------
	// Fields For Testing

	public:
		M_OBJECT_FIELD_VAR_3(private, std::shared_ptr<aiva::layer1_ext::RoMesh>, TestGeometry);

		M_OBJECT_FIELD_VAR_3(private, std::shared_ptr<aiva::layer1_ext::RoMaterialGr>, TestMaterial);

	private:
		void InitFieldsForTesting();

		void ShutFieldsForTesting();
	};
}
