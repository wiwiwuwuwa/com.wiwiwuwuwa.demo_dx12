#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/a_graphic_command.h>
#include <aiva/layer1_ext/gc_set_scissor_rects.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct GcSetScissorRects final : public aiva::utils_ext::TObject<GcSetScissorRects>, public AGraphicCommand
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(GcSetScissorRects);

	protected:
		GcSetScissorRects(aiva::layer1::Engine& engine);

	public:
		~GcSetScissorRects() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_3(public, glm::vec4, Rect);

	private:
		void GraphicCommand_OnExec();

	private:
		void ExecuteRSSetScissorRects();
	};
}
