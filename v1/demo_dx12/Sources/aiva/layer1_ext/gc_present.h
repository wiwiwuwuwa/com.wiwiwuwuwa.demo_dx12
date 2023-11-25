#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/a_graphic_command.h>
#include <aiva/layer1_ext/gc_present_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct GcPresent final : public aiva::utils_ext::TObject<GcPresent>, public AGraphicCommand
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(GcPresent);

	protected:
		GcPresent(aiva::layer1::Engine& engine);

	public:
		~GcPresent() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

	private:
		void GraphicCommand_OnExec();

	private:
		void ExecuteResourceBarrier();
	};
}
