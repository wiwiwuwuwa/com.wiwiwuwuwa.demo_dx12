#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/ro_config_json_fwd.h>
#include <aiva/utils_ext/i_serializable_json.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct RoConfigJson final : public aiva::utils_ext::TObject<RoConfigJson>, public aiva::utils_ext::ISerializableJson
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(RoConfigJson);

	protected:
		RoConfigJson(aiva::layer1::Engine& engine);

	public:
		~RoConfigJson() override;

	// ----------------------------------------------------
	// Serialization

	public:
		void DeserealizeFromJson(nlohmann::json const& resourceJsn) override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, nlohmann::json, Data);
	};
}
