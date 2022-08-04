#pragma once
#include <pch.h>

#include <aiva/layer1/e_resource_memory_type.h>

namespace aiva::layer1
{
	struct GrBufferDesc final
	{
	// ----------------------------------------------------
	// Constructors
		
	public:
		GrBufferDesc();

		GrBufferDesc(winrt::com_ptr<ID3D12Resource> const& resource);

	// ----------------------------------------------------
	// Data Layout

	public:
		EResourceMemoryType MemoryType{};

		std::uint64_t Size{};

	// ----------------------------------------------------
	// Features

	public:
		bool SupportShaderAtomics{};

		bool SupportUnorderedAccess{};
	};
}
