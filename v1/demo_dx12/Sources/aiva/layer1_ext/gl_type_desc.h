#pragma once
#include <pch.h>

#include <aiva/layer1_ext/gl_type_desc_fwd.h>
#include <aiva/utils_ext/m_object_field_var.h>

namespace aiva::layer1_ext
{
	struct GlTypeDesc final
	{
	// ----------------------------------------------------
	// Main

	public:
		GlTypeDesc();

		GlTypeDesc(std::size_t const typeOf, std::size_t const typeIn);

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_VAR_3(public, std::size_t, TypeOf);

		M_OBJECT_FIELD_VAR_3(public, std::size_t, TypeIn);
	};

	bool operator==(GlTypeDesc const& a, GlTypeDesc const& b);

	std::size_t hash_value(GlTypeDesc const& v);
}
