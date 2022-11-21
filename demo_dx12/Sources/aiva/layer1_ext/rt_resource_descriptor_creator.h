#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_resource_descriptor_creator_fwd.h>
#include <aiva/layer1_ext/ut_resources_barriers_emitter.h>
#include <aiva/utils_ext/t_dirty_flags_eager_readonly.h>

namespace aiva::layer1_ext
{
	struct RtResourceDescriptorCreator : public UtResourcesBarriersEmitter
	{
	// ----------------------------------------------------
	// Main

	protected:
		RtResourceDescriptorCreator() = default;

	public:
		virtual ~RtResourceDescriptorCreator() = default;

	// ----------------------------------------------------
	// Generic Fields

	public:
		virtual void CreateResourceDescriptorHandle(D3D12_CPU_DESCRIPTOR_HANDLE const& outHandle) = NULL;

		virtual aiva::utils_ext::TDirtyFlagsEagerReadOnly<>& GetResourceDescriptorDirtyFlags() = NULL;
	};
}
