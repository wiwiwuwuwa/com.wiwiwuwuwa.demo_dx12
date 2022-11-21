#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/a_graphic_command.h>
#include <aiva/layer1_ext/gc_copy_buffer_to_texture_2d_fwd.h>
#include <aiva/layer1_ext/nr_buffer_fwd.h>
#include <aiva/layer1_ext/nr_texture_2d_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct GcCopyBufferToTexture2D final : public aiva::utils_ext::TObject<GcCopyBufferToTexture2D>, public AGraphicCommand
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(GcCopyBufferToTexture2D);

	protected:
		GcCopyBufferToTexture2D(aiva::layer1::Engine& engine);

	public:
		~GcCopyBufferToTexture2D() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_3(public, std::shared_ptr<NrBuffer>, SrcObj);

		M_OBJECT_FIELD_VAR_3(public, std::shared_ptr<NrTexture2D>, DstObj);

	private:
		void GraphicCommand_OnExec();

	private:
		void ExecuteResourceBarrier();

		void ExecuteCopyResource();
	};
}
