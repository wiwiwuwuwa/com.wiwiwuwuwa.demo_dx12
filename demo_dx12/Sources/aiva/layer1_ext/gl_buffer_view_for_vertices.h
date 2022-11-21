#pragma once
#include <pch.h>

#include <aiva/layer1/engine_fwd.h>
#include <aiva/layer1_ext/gl_buffer_view.h>
#include <aiva/utils_ext/e_value_type.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct GlBufferViewForVertices final
	{
	// ----------------------------------------------------
	// Main

	public:
		GlBufferViewForVertices(tinygltf::Model const& gltf, std::size_t const meshIndex, std::size_t const primitiveIndex, aiva::layer1::Engine& engine);

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_VAR_3(private, std::vector<GlBufferView>, BufferViews);

	public:
		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, NumOfElements);

		M_OBJECT_FIELD_VAR_4(public, private, std::vector<aiva::utils_ext::EValueType>, TypeOfElement);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, SizeOfElement);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, SizeOfData);

	public:
		std::vector<std::byte> GetData() const;

		std::vector<std::byte> GetElement(std::size_t const index) const;
	};
}
