#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/utils_ext/i_serializable_json.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct RoTexture2D final : public aiva::utils_ext::TObject<RoTexture2D>, public aiva::utils_ext::ISerializableJson
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY();

	protected:
		RoTexture2D(aiva::layer1::Engine& engine);

	public:
		~RoTexture2D() override;

	// ----------------------------------------------------
	// Serialization

	public:
		void DeserealizeFromJson(nlohmann::json const& resourceJsn) override;

	// ----------------------------------------------------
	// Fields

	public:
		M_OBJECT_FIELD_REF_3(aiva::layer1::Engine, Engine, public);
	};
}
