#pragma once
#include <pch.h>

#include <aiva/layer1/e_gpu_descriptor_heap_type.h>
#include <aiva/utils/ev_action.h>

namespace aiva::layer1
{
	struct IGpuResourceView
	{
	// ----------------------------------------------------
	// Main

	public:
		virtual ~IGpuResourceView() = default;

	// ----------------------------------------------------
	// Metadata

	public:
		virtual EGpuDescriptorHeapType DescriptorHeapType() const = 0;

	// ----------------------------------------------------
	// Events

	public:
		virtual aiva::utils::EvAction& OnInternalResourceUpdated() = 0;
	};
}
