#pragma once
#include <pch.h>

#include <aiva2/engine/engine_fwd.h>
#include <aiva2/engine/graphic_hardware_fwd.h>
#include <aiva2/engine/i_object.h>
#include <aiva2/engine/m_field_by_ref.h>
#include <aiva2/engine/m_struct_body.h>

namespace aiva2::engine
{
	struct GraphicHardware final : public IObject
	{
	// ----------------------------------------------------
	// Main

	public:
		M_STRUCT_BODY(GraphicHardware);

	public:
		GraphicHardware(engine::Engine& engine);

		~GraphicHardware() override;

	public:
		M_FIELD_BY_REF_3(public, engine::Engine, Engine);
	};
}
