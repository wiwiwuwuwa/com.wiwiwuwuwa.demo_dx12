#pragma once
#include <pch.h>

namespace aiva::layer1
{
	struct IShaderResource;
	
	using IShaderResourceType = IShaderResource;
	using IShaderResourceTypeShared = std::shared_ptr<IShaderResourceType>;
	using IShaderResourceTypeWeak = std::weak_ptr<IShaderResourceType>;
}
