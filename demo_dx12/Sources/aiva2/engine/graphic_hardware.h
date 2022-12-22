#pragma once
#include <pch.h>

#include <aiva2/engine/engine_fwd.h>
#include <aiva2/engine/graphic_hardware_fwd.h>
#include <aiva2/engine/i_object.h>
#include <aiva2/engine/m_struct_body.h>

namespace aiva2::engine
{
	struct GraphicHardware : public IObject
	{
	// ----------------------------------------------------
	// Main

	public:
		M_STRUCT_BODY(GraphicHardware);

	public:
		GraphicHardware();

		~GraphicHardware() override;

	// ----------------------------------------------------
	// Engine (Virtual)

	public:
		virtual auto Engine() const -> engine::Engine& = NULL;
	};
}
