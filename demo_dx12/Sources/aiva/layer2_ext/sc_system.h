#pragma once
#include <pch.h>

#include <aiva/layer2/world_fwd.h>
#include <aiva/layer2_ext/sc_actor_fwd.h>
#include <aiva/layer2_ext/sc_scene_fwd.h>
#include <aiva/layer2_ext/sc_system_fwd.h>
#include <aiva/utils_ext/m_object_body.h>
#include <aiva/utils_ext/m_object_field_ref.h>
#include <aiva/utils_ext/m_object_field_var.h>
#include <aiva/utils_ext/t_object.h>

namespace aiva::layer2_ext
{
	struct ScSystem final : public aiva::utils_ext::TObject<ScSystem>
	{
	// ----------------------------------------------------
	// Main

	public:
		M_OBJECT_BODY(ScSystem);

	protected:
		ScSystem(aiva::layer2::World& world);

	public:
		~ScSystem() override;

	// ----------------------------------------------------
	// Generic Fields

	public:
		M_OBJECT_FIELD_REF_3(public, aiva::layer2::World, World);

		M_OBJECT_FIELD_VAR_3(private, std::shared_ptr<ScScene>, Scene);
	};
}
