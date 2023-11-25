#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/ro_image_png_fwd.h>
#include <aiva/utils_ext/i_serializable_binary.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_val_dirty_flags_eager.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer1_ext
{
	struct RoImagePng final : public aiva::utils_ext::TObject<RoImagePng>, public aiva::utils_ext::ISerializableBinary
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(RoImagePng);

	protected:
		RoImagePng(aiva::layer1::Engine& engine);

	public:
		~RoImagePng() override;

	// ----------------------------------------------------
	// Serialization

	public:
		void DeserealizeFromBinary(boost::span<const std::byte> const& resourceBin) override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer1::Engine, Engine);

		M_OBJECT_FIELD_VAL_DIRTY_FLAGS_EAGER();

	// ----------------------------------------------------
	// Params Fields

	public:
		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, Width);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, Height);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, Channels);

		M_OBJECT_FIELD_VAR_4(public, private, std::shared_ptr<std::byte>, Data);

	public:
		boost::span<const std::byte> Span() const;
	};
}
