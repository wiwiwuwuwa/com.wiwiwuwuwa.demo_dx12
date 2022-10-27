#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct MaterialResourceDescriptor;

	using MaterialResourceDescriptorType = MaterialResourceDescriptor;
	using MaterialResourceDescriptorTypeShared = std::shared_ptr<MaterialResourceDescriptorType>;
	using MaterialResourceDescriptorTypeWeak = std::weak_ptr<MaterialResourceDescriptorType>;
}
