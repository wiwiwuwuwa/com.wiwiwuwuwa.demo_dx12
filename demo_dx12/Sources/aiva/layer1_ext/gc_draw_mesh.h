#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/a_graphic_command.h>
#include <aiva/layer1_ext/gc_draw_mesh_fwd.h>
#include <aiva/layer1_ext/ro_material_gr_fwd.h>
#include <aiva/layer1_ext/ro_mesh_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct GcDrawMesh final : public aiva::utils_ext::TObject<GcDrawMesh>, public AGraphicCommand
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(GcDrawMesh);

	protected:
		GcDrawMesh(aiva::layer1::Engine& engine);

	public:
		~GcDrawMesh() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_3(public, std::shared_ptr<RoMesh>, Geometry);

		M_OBJECT_FIELD_VAR_3(public, std::shared_ptr<RoMaterialGr>, Material);

		M_OBJECT_FIELD_VAR_5(public, private, std::size_t, Instance, 1);

	private:
		void GraphicCommand_OnExec();

	private:
		void ExecuteResourceBarrier();

		void ExecuteSetPipelineState();

		void ExecuteSetGraphicRootSignature();

		void ExecuteSetDescriptorHeaps();

		void ExecuteSetGraphicsRootDescriptorTable();

		void ExecuteIASetPrimitiveTopology();

		void ExecuteIASetIndexBuffer();

		void ExecuteIASetVertexBuffers();

		void ExecuteDrawIndexedInstanced();
	};
}
