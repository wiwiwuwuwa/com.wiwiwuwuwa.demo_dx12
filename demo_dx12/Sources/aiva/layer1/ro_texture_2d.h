#pragma once
#include <pch.h>

#include <aiva/layer1/i_object_engineable.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/m_object_body.h>

namespace aiva::layer1
{
	struct RoTexture2D final : public aiva::utils::AObject, public aiva::layer1::IObjectEngineable
	{
	// ----------------------------------------------------
	// Base

	M_OBJECT_BODY();

	// ----------------------------------------------------
	// Main

	private:
		RoTexture2D(EngineType const& engine);

	public:
		~RoTexture2D() override;
	};
}
