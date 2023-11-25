#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/nr_buffer_fwd.h>
#include <aiva/layer1_ext/ro_mesh_gltf_fwd.h>
#include <aiva/layer1_ext/ut_resources_barriers_emitter.h>
#include <aiva/utils_ext/e_value_type.h>
#include <aiva/utils_ext/i_serializable_json.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct RoMesh final : public aiva::utils_ext::TObject<RoMesh>, public aiva::utils_ext::ISerializableJson, public UtResourcesBarriersEmitter
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(RoMesh);

	protected:
		RoMesh(aiva::layer1::Engine& engine);

	public:
		~RoMesh() override;

	// ----------------------------------------------------
	// Serialization

	public:
		void DeserealizeFromJson(nlohmann::json const& resourceJsn) override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAR_3(private, std::shared_ptr<NrBuffer>, IndexBuffer);

		M_OBJECT_FIELD_VAR_3(private, std::shared_ptr<NrBuffer>, VertexBuffer);

	public:
		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, IndexCount);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, VertexCount);

	public:
		M_OBJECT_FIELD_VAR_4(public, private, D3D12_INDEX_BUFFER_VIEW, NativeIndexBufferView);

		M_OBJECT_FIELD_VAR_4(public, private, D3D12_VERTEX_BUFFER_VIEW, NativeVertexBufferView);

	// ----------------------------------------------------
	// Resources Barriers Emitter

	public:
		std::vector<D3D12_RESOURCE_BARRIER> TransiteResourceBarriers(UtBarriersFlags const flags) override;
	};
}
