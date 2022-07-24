#pragma once
#include <pch.h>

#include <aiva/layer1/i_shader_value.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	template <typename TValue>
	struct TShaderArray final : private boost::noncopyable, public std::enable_shared_from_this<TShaderArray<TValue>>, public IShaderValue
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<TShaderArray> Create(TArgs&&... args);

	private:
		TShaderArray(std::vector<TValue> const& value);

	public:
		~TShaderArray();

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
template <typename... TArgs>
std::shared_ptr<aiva::layer1::TShaderArray<TValue>> aiva::layer1::TShaderArray<TValue>::Create(TArgs&&... args)
{
	return std::shared_ptr<TShaderArray>{new TShaderArray{ std::forward<TArgs>(args)... }};
}

template <typename TValue>
aiva::layer1::TShaderArray<TValue>::TShaderArray(std::vector<TValue> const& value) : mValues{ value }
{

}

template <typename TValue>
aiva::layer1::TShaderArray<TValue>::~TShaderArray()
{

}

template <typename TValue>
std::vector<std::byte> aiva::layer1::TShaderArray<TValue>::SerializeToBinary() const
{
	std::size_t const binaryValueSize = aiva::utils::MathUtils::Align(sizeof(TValue), ShaderConsts::VECTOR_SIZE);
	std::size_t const binaryArraySize = binaryValueSize * mValues.size();

	std::vector<std::byte> binaryData{ binaryArraySize };
	for (std::size_t valueIndex = 0; valueIndex < mValues.size(); valueIndex++)
	{
		std::size_t const binaryIndex = valueIndex * binaryValueSize;
		aiva::utils::Asserts::CheckBool(memcpy_s(binaryData.data() + binaryIndex, binaryValueSize, &mValues.at(valueIndex), sizeof(mValues.at(valueIndex))) == 0);
	}

	return binaryData;
}

template <typename TValue>
std::vector<TValue> const& aiva::layer1::TShaderArray<TValue>::Value() const
{
	return mValues;
}
