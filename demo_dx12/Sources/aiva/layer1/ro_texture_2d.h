#pragma once
#include <pch.h>

#include <aiva/layer1/a_resource_object.h>
#include <aiva/layer1/m_resource_object_body.h>

namespace aiva::layer1
{
	struct RoTexture2D final : public AResourceObject
	{
	// ----------------------------------------------------
	// Base

	M_RESOURCE_OBJECT_BODY();

	// ----------------------------------------------------
	// Main

	private:
		RoTexture2D(EngineType const& engine);

	public:
		~RoTexture2D() override;
	};
}
