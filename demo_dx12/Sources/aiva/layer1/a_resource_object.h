#pragma once
#include <pch.h>

#include <aiva/layer1/i_object_engineable.h>
#include <aiva/utils/a_object.h>

namespace aiva::layer1
{
	struct AResourceObject : public aiva::utils::AObject, public aiva::layer1::IObjectEngineable
	{
	// ----------------------------------------------------
	// Main

	protected:
		AResourceObject(EngineType const& engine);

	public:
		~AResourceObject() override;
	};
}
