#pragma once
#include <pch.h>

#include <aiva/layer1_ext/rt_resource_heap_container_fwd.h>
#include <aiva/layer1_ext/ut_resources_barriers_emitter.h>
#include <aiva/utils_ext/t_dirty_flags_eager_readonly.h>

namespace aiva::layer1_ext
{
	struct RtResourceHeapContainer : public UtResourcesBarriersEmitter
	{
	// ----------------------------------------------------
	// Main

	protected:
		RtResourceHeapContainer() = default;

	public:
		virtual ~RtResourceHeapContainer() = default;

	// ----------------------------------------------------
	// Generic Fields

	public:
		virtual winrt::com_ptr<ID3D12DescriptorHeap> const& GetResourceHeapData() = NULL;

		virtual aiva::utils_ext::TDirtyFlagsEagerReadOnly<>& GetResourceHeapDirtyFlags() = NULL;
	};
}
