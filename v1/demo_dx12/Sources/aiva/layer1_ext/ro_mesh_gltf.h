#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/ro_mesh_gltf_fwd.h>
#include <aiva/utils_ext/e_value_type.h>
#include <aiva/utils_ext/i_serializable_json.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct RoMeshGltf final : public aiva::utils_ext::TObject<RoMeshGltf>, public aiva::utils_ext::ISerializableJson
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(RoMeshGltf);

	protected:
		RoMeshGltf(aiva::layer1::Engine& engine);

		RoMeshGltf(nlohmann::json const& resourceJsn, aiva::layer1::Engine& engine);

	public:
		~RoMeshGltf() override;

	// ----------------------------------------------------
	// Serialization

	public:
		void DeserealizeFromJson(nlohmann::json const& resourceJsn) override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, std::vector<std::byte>, IndicesData);

		M_OBJECT_FIELD_VAR_4(public, private, aiva::utils_ext::EValueType, IndicesType);

		M_OBJECT_FIELD_VAR_4(public, private, std::vector<std::byte>, VerticesData);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, VerticesStride);
	};
}
