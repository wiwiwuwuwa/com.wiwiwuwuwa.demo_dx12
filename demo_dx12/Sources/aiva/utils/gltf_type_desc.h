#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct GltfTypeDesc final
	{
		std::size_t TypeOfComponent{};

		std::size_t TypeInComponent{};
	};

	bool operator==(GltfTypeDesc const& a, GltfTypeDesc const& b);

	std::size_t hash_value(GltfTypeDesc const& typeDesc);
}
