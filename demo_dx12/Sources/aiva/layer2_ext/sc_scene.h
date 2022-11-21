#pragma once
#include <pch.h>

#include <aiva/layer2/world_fwd.h>
#include <aiva/layer2_ext/sc_actor_fwd.h>
#include <aiva/layer2_ext/sc_scene_fwd.h>
#include <aiva/utils_ext/i_serializable_json.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer2_ext
{
	struct ScScene final : public aiva::utils_ext::TObject<ScScene>, public aiva::utils_ext::ISerializableJson
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ScScene);

	protected:
		ScScene(aiva::layer2::World& world);

	public:
		~ScScene() override;

	// ----------------------------------------------------
	// Serialization

	public:
		void DeserealizeFromJson(nlohmann::json const& resourceJsn) override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer2::World, World);

		M_OBJECT_FIELD_VAR_3(private, std::shared_ptr<ScActor>, Actor);
	};
}
