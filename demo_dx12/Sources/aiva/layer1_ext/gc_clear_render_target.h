#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/a_graphic_command.h>
#include <aiva/layer1_ext/gc_clear_render_target_fwd.h>
#include <aiva/layer1_ext/rt_render_identity_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct GcClearRenderTarget final : public aiva::utils_ext::TObject<GcClearRenderTarget>, public AGraphicCommand
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(GcClearRenderTarget);

	protected:
		GcClearRenderTarget(aiva::layer1::Engine& engine);

	public:
		~GcClearRenderTarget() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, std::shared_ptr<RtRenderIdentity<RtResourceViewType::Rtv>>, Target);

		M_OBJECT_FIELD_VAR_3(public, glm::vec4, Color);

	private:
		void GraphicCommand_OnExec();

	private:
		void ExecuteResourceBarrier();

		void ExecuteClearRenderTargetView();
	};
}
