#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/a_graphic_command.h>
#include <aiva/layer1_ext/gc_set_viewports_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct GcSetViewports final : public aiva::utils_ext::TObject<GcSetViewports>, public AGraphicCommand
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(GcSetViewports);

	protected:
		GcSetViewports(aiva::layer1::Engine& engine);

	public:
		~GcSetViewports() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_3(public, glm::vec4, Rect);

	private:
		void GraphicCommand_OnExec();

	private:
		void ExecuteRSSetViewports();
	};
}
