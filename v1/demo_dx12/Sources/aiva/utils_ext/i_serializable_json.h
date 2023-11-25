#pragma once
#include <pch.h>

namespace aiva::utils_ext
{
	struct ISerializableJson
	{
	// ----------------------------------------------------
	// Main

	public:
		virtual void DeserealizeFromJson(nlohmann::json const&) = NULL;

		virtual ~ISerializableJson() = default;
	};
}
