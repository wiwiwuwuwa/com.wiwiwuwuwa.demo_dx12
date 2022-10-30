#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/utils_ext/i_serializable_binary.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ptr.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_val.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_eager.h>
#include <aiva/utils_ext/t_dirty_flags_eager.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct RoImagePng final : public aiva::utils_ext::TObject<RoImagePng>, public aiva::utils_ext::ISerializableBinary
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY();

	protected:
		RoImagePng(aiva::layer1::Engine& engine);

	public:
		~RoImagePng() override;

	// ----------------------------------------------------
	// Serialization

	public:
		void DeserealizeFromBinary(boost::span<const std::byte> const& resourceBin) override;

	// ----------------------------------------------------
	// Fields

	public:
		M_OBJECT_FIELD_REF_3(aiva::layer1::Engine, Engine, public);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER_2(OnUpdate, public);

		M_OBJECT_FIELD_VAL_4(std::size_t, Width, public, private);
		
		M_OBJECT_FIELD_VAL_4(std::size_t, Height, public, private);

		M_OBJECT_FIELD_VAL_4(std::size_t, Channels, public, private);

		M_OBJECT_FIELD_PTR_4(std::byte, Data, private, private);
	};
}
