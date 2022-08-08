#pragma once
#include <pch.h>

#include <aiva/layer1/i_shader_value.h>
#include <aiva/layer1/shader_consts.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/math_utils.h>

namespace aiva::utils
{
	struct ObjectFactory;
}

namespace aiva::layer1
{
	template <typename TValue>
	struct TShaderArray final : public aiva::utils::AObject, public IShaderValue
	{
	// ----------------------------------------------------
	// Main

	private:
		friend aiva::utils::ObjectFactory;

	private:
		TShaderArray(std::vector<TValue> const& value);

	public:
		~TShaderArray() override;

	// ----------------------------------------------------
	// IShaderValue

	public:
		std::vector<std::byte> SerializeToBinary() const override;

	// ----------------------------------------------------
	// High-Level Data

	public:
		std::vector<TValue> const& Value() const;

	private:
		std::vector<TValue> mValues{};
	};
}

// --------------------------------------------------------

template <typename TValue>
aiva::layer1::TShaderArray<TValue>::TShaderArray(std::vector<TValue> const& value) : aiva::utils::AObject{}, mValues{ value }
{

}

template <typename TValue>
aiva::layer1::TShaderArray<TValue>::~TShaderArray()
{

}

template <typename TValue>
std::vector<std::byte> aiva::layer1::TShaderArray<TValue>::SerializeToBinary() const
{
	auto const binaryValueSize = aiva::utils::MathUtils::Align(sizeof(TValue), ShaderConsts::VECTOR_SIZE);
	auto const binaryArraySize = binaryValueSize * mValues.size();

	//std::vector<std::byte> binaryData{ binaryArraySize };
	auto binaryData = std::vector<std::byte>{ binaryArraySize };
	for (std::size_t valueIndex = 0; valueIndex < mValues.size(); valueIndex++)
	{
		auto const binaryIndex = valueIndex * binaryValueSize;
		aiva::utils::Asserts::CheckBool(memcpy_s(binaryData.data() + binaryIndex, binaryValueSize, &mValues.at(valueIndex), sizeof(mValues.at(valueIndex))) == 0);
	}

	return binaryData;
}

template <typename TValue>
std::vector<TValue> const& aiva::layer1::TShaderArray<TValue>::Value() const
{
	return mValues;
}
