#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/ro_shader_vert_fwd.h>
#include <aiva/layer1_ext/shader_ia_input_layout_desc_fwd.h>
#include <aiva/layer1_ext/shader_meta_for_code_fwd.h>
#include <aiva/utils_ext/i_serializable_binary.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_eager.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct RoShaderVert final : public aiva::utils_ext::TObject<RoShaderVert>, public aiva::utils_ext::ISerializableBinary
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(RoShaderVert);

	protected:
		RoShaderVert(aiva::layer1::Engine& engine);

	public:
		~RoShaderVert() override;

	// ----------------------------------------------------
	// Serialization

	public:
		void DeserealizeFromBinary(boost::span<const std::byte> const& resourceBin) override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER();

		M_OBJECT_FIELD_VAR_4(public, private, winrt::com_ptr<ID3DBlob>, Bytecode);

		M_OBJECT_FIELD_VAR_4(public, private, std::shared_ptr<ShaderMetaForCode>, Metadata);

		M_OBJECT_FIELD_VAR_4(public, private, std::shared_ptr<ShaderIAInputLayoutDesc>, InputLayout);
	};
}
