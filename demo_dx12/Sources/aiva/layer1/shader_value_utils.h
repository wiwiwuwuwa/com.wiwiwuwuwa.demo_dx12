#pragma once
#include <pch.h>

namespace aiva::layer1
{
	enum class EValueType;
	struct IShaderValue;

	template <typename>
	struct TShaderValue;
}

namespace aiva::layer1
{
	struct ShaderValueUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		ShaderValueUtils();

		~ShaderValueUtils();

	private:
		static ShaderValueUtils& Instance();

	// ----------------------------------------------------
	// GLTF Conversions

	public:
		static std::shared_ptr<IShaderValue> CreateFromGltf(int const glScalarCount, int const glScalarType);

	// ----------------------------------------------------
	// Value Type Conversions

	public:
		static std::shared_ptr<IShaderValue> CreateFromValueType(EValueType const valueType);

	private:
		using ValueTypeFactoryMethod = std::function<std::shared_ptr<IShaderValue>()>;

	private:
		void InitializeValueTypeFactories();

		void TerminateValueTypeFactories();

	private:
		template <typename T>
		void RegisterValueTypeFactory(EValueType const valueType);

	private:
		std::unordered_map<EValueType, ValueTypeFactoryMethod> mValueTypeFactories{};
	};
}

// --------------------------------------------------------

template <typename T>
void aiva::layer1::ShaderValueUtils::RegisterValueTypeFactory(EValueType const valueType)
{
	mValueTypeFactories.insert_or_assign(valueType, &TShaderValue<T>::Create<>);
}
