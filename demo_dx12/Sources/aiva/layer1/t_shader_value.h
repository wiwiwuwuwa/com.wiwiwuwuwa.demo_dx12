#pragma once
#include <pch.h>

#include <aiva/layer1/i_shader_value.h>
#include <aiva/layer1/shader_consts.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/math_utils.h>

namespace aiva::layer1
{
	template <typename TValue>
	struct TShaderValue final : public aiva::utils::AObject, public IShaderValue
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	private:
		TShaderValue(TValue const& value = {});

	public:
		~TShaderValue() override;

	// ----------------------------------------------------
	// IShaderValue

	public:
		std::vector<std::byte> SerializeToBinary() const override;

		void DeserealizeFromBinary(boost::span<const std::byte> const& binary) override;

		std::size_t MinBinarySize() const override;

	// ----------------------------------------------------
	// High-Level Data

	public:
		TValue const& Value() const;

	private:
		TValue mValue{};
	};
}

// --------------------------------------------------------

template <typename TValue>
aiva::layer1::TShaderValue<TValue>::TShaderValue(TValue const& value /*={}*/) : aiva::utils::AObject{}, mValue{ value }
{

}

template <typename TValue>
aiva::layer1::TShaderValue<TValue>::~TShaderValue()
{

}

template <typename TValue>
std::vector<std::byte> aiva::layer1::TShaderValue<TValue>::SerializeToBinary() const
{
	auto const binarySize = aiva::utils::MathUtils::Align(sizeof(TValue), ShaderConsts::FLOAT_SIZE);

	auto binaryData = std::vector<std::byte>{ binarySize };
	aiva::utils::Asserts::CheckBool(memcpy_s(binaryData.data(), binaryData.size(), &mValue, sizeof(mValue)) == 0);

	return binaryData;
}

template <typename TValue>
void aiva::layer1::TShaderValue<TValue>::DeserealizeFromBinary(boost::span<const std::byte> const& binary)
{
	aiva::utils::Asserts::CheckBool(!std::empty(binary), "Binary data is empty");
	aiva::utils::Asserts::CheckBool(std::size(binary) >= sizeof(TValue), "Binary data is too small");

	aiva::utils::Asserts::CheckBool(memcpy_s(&mValue, sizeof(mValue), std::data(binary), std::size(binary)) == 0, "Failed to memcpy_s");
}

template <typename TValue>
TValue const& aiva::layer1::TShaderValue<TValue>::Value() const
{
	return mValue;
}

template <typename TValue>
std::size_t aiva::layer1::TShaderValue<TValue>::MinBinarySize() const
{
	return sizeof(TValue);
}
