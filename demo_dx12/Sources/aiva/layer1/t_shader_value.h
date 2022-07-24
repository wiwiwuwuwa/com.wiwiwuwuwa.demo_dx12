#pragma once
#include <pch.h>

#include <aiva/layer1/i_shader_value.h>
#include <aiva/layer1/shader_consts.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/math_utils.h>

namespace aiva::layer1
{
	template <typename TValue>
	struct TShaderValue final : private boost::noncopyable, public std::enable_shared_from_this<TShaderValue<TValue>>, public IShaderValue
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<TShaderValue> Create(TArgs&&... args);

	private:
		TShaderValue(TValue const& value);

	public:
		~TShaderValue();

	// ----------------------------------------------------
	// IShaderValue

	public:
		std::vector<std::byte> SerializeToBinary() const override;

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
template <typename... TArgs>
std::shared_ptr<aiva::layer1::TShaderValue<TValue>> aiva::layer1::TShaderValue<TValue>::Create(TArgs&&... args)
{
	return std::shared_ptr<TShaderValue>{new TShaderValue{ std::forward<TArgs>(args)... }};
}

template <typename TValue>
aiva::layer1::TShaderValue<TValue>::TShaderValue(TValue const& value) : mValue{ value }
{

}

template <typename TValue>
aiva::layer1::TShaderValue<TValue>::~TShaderValue()
{

}

template <typename TValue>
std::vector<std::byte> aiva::layer1::TShaderValue<TValue>::SerializeToBinary() const
{
	std::size_t const binarySize = aiva::utils::MathUtils::Align(sizeof(TValue), ShaderConsts::FLOAT_SIZE);

	std::vector<std::byte> binaryData{ binarySize };
	aiva::utils::Asserts::CheckBool(memcpy_s(binaryData.data(), binaryData.size(), &mValue, sizeof(mValue)) == 0);

	return binaryData;
}

template <typename TValue>
TValue const& aiva::layer1::TShaderValue<TValue>::Value() const
{
	return mValue;
}
