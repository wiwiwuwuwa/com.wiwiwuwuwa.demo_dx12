#pragma once
#include <pch.h>

#include <aiva/utils/e_boxed_type.h>
#include <aiva/utils/gltf_type_desc.h>

namespace aiva::utils
{
	struct BoxedTypeUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		BoxedTypeUtils();

		~BoxedTypeUtils();

	private:
		static BoxedTypeUtils& Instance();

	// ----------------------------------------------------
	// GLTF Parsing

	public:
		static EBoxedType Parse(GltfTypeDesc const& desc);

	private:
		void InitializeGltfParsing();

		void TerminateGltfParsing();

	private:
		std::unordered_map<GltfTypeDesc, EBoxedType, boost::hash<GltfTypeDesc>> mGltfParsingValues{};
	};
}
