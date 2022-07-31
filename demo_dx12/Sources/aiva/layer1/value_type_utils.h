#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EValueType;
}

namespace aiva::layer1
{
	struct ValueTypeUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		ValueTypeUtils();

		~ValueTypeUtils();

	private:
		static ValueTypeUtils& Instance();

	// ----------------------------------------------------
	// GLTF Conversions

	public:
		static EValueType ParseFromGltf(int const glScalarCount, int const glScalarType);

	private:
		static const std::string KEY_SEP;

		static std::string CreateKeyFromGltf(int const glScalarCount, int const glScalarType);

	private:
		void InitializeGltfFactories();

		void TerminateGltfFactories();

	private:
		std::unordered_map<std::string, EValueType> mGltfFactories{};
	};
}
