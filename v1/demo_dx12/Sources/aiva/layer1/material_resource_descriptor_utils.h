#pragma once
#include <pch.h>

#include <aiva/layer1/i_shader_resource_fwd.h>
#include <aiva/layer1/material_resource_descriptor_fwd.h>

namespace aiva::layer1
{
	struct MaterialResourceDescriptorUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		MaterialResourceDescriptorUtils() = delete;

	// ----------------------------------------------------
	// Shader Resource Utils

	public:
		static void SetSharedResource(MaterialResourceDescriptorTypeShared const& dstDescriptor, std::string const& srcVariableName, IShaderResourceTypeShared const& srcShaderResource);
	};
}
