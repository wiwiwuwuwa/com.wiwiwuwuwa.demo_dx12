#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/a_graphic_command.h>
#include <aiva/layer1_ext/gc_copy_buffer_to_buffer_fwd.h>
#include <aiva/layer1_ext/nr_buffer_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct GcCopyBufferToBuffer final : public aiva::utils_ext::TObject<GcCopyBufferToBuffer>, public AGraphicCommand
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(GcCopyBufferToBuffer);

	protected:
		GcCopyBufferToBuffer(aiva::layer1::Engine& engine);

	public:
		~GcCopyBufferToBuffer() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_3(public, std::shared_ptr<NrBuffer>, Src);

		M_OBJECT_FIELD_VAR_3(public, std::shared_ptr<NrBuffer>, Dst);

	private:
		void GraphicCommand_OnExec();

	private:
		void ExecuteResourceBarrier();

		void ExecuteCopyResource();
	};
}
