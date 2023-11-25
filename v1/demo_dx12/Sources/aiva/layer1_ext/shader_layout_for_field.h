#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/shader_layout_for_field_fwd.h>
#include <aiva/layer1_ext/shader_layout_for_type_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_field_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct ShaderLayoutForField final : public aiva::utils_ext::TObject<ShaderLayoutForField>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ShaderLayoutForField);

	protected:
		ShaderLayoutForField(std::shared_ptr<ShaderMetaForField> const& metaForField, std::size_t const offset, aiva::layer1::Engine& engine);

	public:
		~ShaderLayoutForField() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, Offset);

		M_OBJECT_FIELD_VAR_4(public, private, std::shared_ptr<ShaderLayoutForType>, Type);

		M_OBJECT_FIELD_VAR_4(public, private, bool, IsPartOfLayout);
	};
}
