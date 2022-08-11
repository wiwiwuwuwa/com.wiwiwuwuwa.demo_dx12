#include <pch.h>
#include <aiva/utils/gltf_type_desc.h>

namespace aiva::utils
{
	bool operator==(GltfTypeDesc const& a, GltfTypeDesc const& b)
	{
		auto equals = true;
		equals &= (a.TypeOfComponent == b.TypeOfComponent);
		equals &= (a.TypeInComponent == b.TypeInComponent);
		return equals;
	}

	std::size_t hash_value(GltfTypeDesc const& typeDesc)
	{
		auto typeHash = std::size_t{};
		boost::hash_combine(typeHash, typeDesc.TypeOfComponent);
		boost::hash_combine(typeHash, typeDesc.TypeInComponent);
		return typeHash;
	}
}
