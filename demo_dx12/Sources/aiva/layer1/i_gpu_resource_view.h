#pragma once
#include <pch.h>

#include <aiva/layer1/e_gpu_descriptor_heap_type.h>

namespace aiva::layer1
{
	struct IGpuResourceView
	{
	public:
		virtual ~IGpuResourceView() = default;

		virtual EGpuDescriptorHeapType DescriptorHeapType() const = 0;
	};
}
