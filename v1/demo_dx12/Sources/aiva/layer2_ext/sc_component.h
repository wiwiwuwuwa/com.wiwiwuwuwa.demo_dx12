#pragma once
#include <pch.h>

#include <aiva/layer2_ext/sc_actor_fwd.h>
#include <aiva/layer2_ext/sc_component_fwd.h>
#include <aiva/utils_ext/m_object_field_ref.h>

namespace aiva::layer2_ext
{
	struct ScComponent
	{
	// ----------------------------------------------------
	// Main

	public:
		using ThisType = ScComponent;

	protected:
		ScComponent(ScActor& parent);

	public:
		virtual ~ScComponent();

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, ScActor, Parent);
	};
}
