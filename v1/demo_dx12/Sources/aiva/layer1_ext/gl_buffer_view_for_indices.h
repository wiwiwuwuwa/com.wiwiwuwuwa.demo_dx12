#pragma once
#include <pch.h>

#include <aiva/layer1_ext/gl_buffer_view.h>
#include <aiva/utils_ext/e_value_type.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct GlBufferViewForIndices final
	{
	// ----------------------------------------------------
	// Main

	public:
		GlBufferViewForIndices(tinygltf::Model const& gltf, std::size_t const meshIndex, std::size_t const primitiveIndex);

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_VAR_3(private, GlBufferView, BufferView);

	public:
		std::size_t SizeOfData() const;

		std::vector<std::byte> GetData() const;

	public:
		std::size_t NumOfElements() const;

		aiva::utils_ext::EValueType TypeOfElement() const;

		std::size_t SizeOfElement() const;

		std::vector<std::byte> GetElement(std::size_t const index) const;
	};
}