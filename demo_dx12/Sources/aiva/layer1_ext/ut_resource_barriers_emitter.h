#pragma once
#include <pch.h>

#include <aiva/layer1_ext/ut_resource_barriers_emitter_fwd.h>

namespace aiva::layer1_ext
{
	struct UtResourceBarriersEmitter
	{
	// ----------------------------------------------------
	// Main

	protected:
		UtResourceBarriersEmitter() = default;

	public:
		virtual ~UtResourceBarriersEmitter() = default;

	// ----------------------------------------------------
	// Generic Fields

	public:
		virtual std::vector<D3D12_RESOURCE_BARRIER> TransiteResourceBarriers(std::optional<std::size_t> const subResource, D3D12_RESOURCE_STATES const desiredState) = NULL;
	};
}
