#pragma once
#include <pch.h>

#include <aiva/layer1_ext/ut_barriers_flags.h>
#include <aiva/layer1_ext/ut_resources_barriers_emitter_fwd.h>

namespace aiva::layer1_ext
{
	struct UtResourcesBarriersEmitter
	{
	// ----------------------------------------------------
	// Main

	protected:
		UtResourcesBarriersEmitter() = default;

	public:
		virtual ~UtResourcesBarriersEmitter() = default;

	// ----------------------------------------------------
	// Generic Fields

	public:
		virtual std::vector<D3D12_RESOURCE_BARRIER> TransiteResourceBarriers(UtBarriersFlags const flags) = NULL;
	};
}
