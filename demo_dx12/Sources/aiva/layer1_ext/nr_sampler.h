#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/nr_sampler_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_eager.h>
#include <aiva/utils_ext/m_object_field_var_custom.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct NrSampler final : public aiva::utils_ext::TObject<NrSampler>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(NrSampler);

	protected:
		NrSampler(aiva::layer1::Engine& engine);

	public:
		~NrSampler() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER();
	};
}
