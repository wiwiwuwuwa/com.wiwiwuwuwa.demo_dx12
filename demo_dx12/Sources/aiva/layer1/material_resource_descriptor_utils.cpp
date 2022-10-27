#include <pch.h>
#include <aiva/layer1/material_resource_descriptor_utils.h>

#include <aiva/layer1/i_shader_resource.h>
#include <aiva/layer1/material_resource_descriptor.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	using namespace aiva::utils;

	void MaterialResourceDescriptorUtils::SetSharedResource(MaterialResourceDescriptorTypeShared const& dstDescriptor, std::string const& srcVariableName, IShaderResourceTypeShared const& srcShaderResource)
	{
		Asserts::CheckBool(dstDescriptor, "Dst descriptor is not valid");
		Asserts::CheckBool(!std::empty(srcVariableName), "Src variable name is not valid");

		Asserts::CheckBool(false, "TODO: Implement me");
	}
}
