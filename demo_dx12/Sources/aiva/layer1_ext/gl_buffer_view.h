#pragma once
#include <pch.h>

#include <aiva/utils_ext/e_value_type.h>
#include <aiva/utils_ext/m_object_field_ptr.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct GlBufferView final
	{
	// ----------------------------------------------------
	// Main

	public:
		GlBufferView() = default;

		GlBufferView(tinygltf::Model const& gltf, std::size_t const accessorIndex);

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_PTR_4(tinygltf::Model const, GLTF, private, private);

		M_OBJECT_FIELD_VAR_3(private, std::size_t, AccessorIndex);

	public:
		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, NumOfElements);

		M_OBJECT_FIELD_VAR_4(public, private, aiva::utils_ext::EValueType, TypeOfElement);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, SizeOfElement);

		M_OBJECT_FIELD_VAR_4(public, private, std::size_t, SizeOfData);

	public:
		std::vector<std::byte> GetData() const;

		std::vector<std::byte> GetElement(std::size_t const index) const;
	};
}
