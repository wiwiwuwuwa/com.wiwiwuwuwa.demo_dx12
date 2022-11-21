#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/ro_scene_gltf_fwd.h>
#include <aiva/utils_ext/i_serializable_binary.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct RoSceneGltf final : public aiva::utils_ext::TObject<RoSceneGltf>, public aiva::utils_ext::ISerializableBinary
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(RoSceneGltf);

	protected:
		RoSceneGltf(aiva::layer1::Engine& engine);

	public:
		~RoSceneGltf() override;

	// ----------------------------------------------------
	// Serialization

	public:
		void DeserealizeFromBinary(boost::span<const std::byte> const& resourceBin) override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_4(public, private, tinygltf::Model, Data);
	};
}
