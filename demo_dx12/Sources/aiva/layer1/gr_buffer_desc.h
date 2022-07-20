#pragma once
#include <pch.h>

#include <aiva/layer1/e_gpu_resource_memory_type.h>

namespace aiva::layer1
{
	struct GrBufferDesc final
	{
	// ----------------------------------------------------
	// Data Layout

	public:
		EGpuResourceMemoryType MemoryType{};

		std::uint64_t Size{};

	// ----------------------------------------------------
	// Features

	public:
		bool SupportShaderAtomics{};

		bool SupportUnorderedAccess{};
	};
}
