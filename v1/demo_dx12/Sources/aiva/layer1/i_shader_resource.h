#pragma once
#include <pch.h>

#include <aiva/layer1/i_shader_resource_fwd.h>

namespace aiva::layer1
{
	struct IShaderResource
	{
	// ----------------------------------------------------
	// Main

	public:
		virtual ~IShaderResource() = default;
	};
}
