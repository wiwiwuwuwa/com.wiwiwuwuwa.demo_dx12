#pragma once
#include <pch.h>

#include <aiva/layer1/i_shader_value.h>
#include <aiva/layer1/t_shader_array.h>
#include <aiva/layer1/t_shader_value.h>
#include <aiva/utils/asserts.h>

namespace aiva::layer1
{
	struct ShaderStruct final : private boost::noncopyable, public std::enable_shared_from_this<ShaderStruct>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<ShaderStruct> Create(TArgs&&... args);

	private:
		ShaderStruct();

	public:
		~ShaderStruct();

	// ----------------------------------------------------
	// Serialization

	public:
		std::vector<std::byte> SerializeToBinary() const;

	// ----------------------------------------------------
	// High-Level Data

	public:
		template <typename TValue>
		bool GetValue(std::string const& key, TValue *const value) const;

		template <typename TValue>
		void SetValue(std::string const& key, TValue const*const value);

		template <typename TValue>
		bool GetArray(std::string const& key, std::vector<TValue> *const value) const;

		template <typename TValue>
		void SetArray(std::string const& key, std::vector<TValue> const*const value);

		bool HasSameFields(ShaderStruct const& other) const;

	private:
		std::unordered_map<std::string, std::shared_ptr<IShaderValue>> mValues{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::ShaderStruct> aiva::layer1::ShaderStruct::Create(TArgs&&... args)
{
	return std::shared_ptr<ShaderStruct>{new ShaderStruct{ std::forward<TArgs>(args)... }};
}

template <typename TValue>
bool aiva::layer1::ShaderStruct::GetValue(std::string const& key, TValue *const value) const
{
	aiva::utils::Asserts::CheckBool(value);

	auto const& valueIter = mValues.find(key);
	if (valueIter == mValues.end())
	{
		*value = {};
		return false;
	}

	auto const& basicValue = valueIter->second;
	aiva::utils::Asserts::CheckBool(basicValue);

	auto const& specificValue = std::dynamic_pointer_cast<TShaderValue<TValue>>(basicValue);
	aiva::utils::Asserts::CheckBool(specificValue);

	*value = specificValue->Value();
	return true;
}

template <typename TValue>
void aiva::layer1::ShaderStruct::SetValue(std::string const& key, TValue const*const value)
{
	if (value)
	{
		auto const& specificValue = TShaderValue<TValue>::Create(*value);
		aiva::utils::Asserts::CheckBool(specificValue);

		mValues.insert_or_assign(key, specificValue);
		return;
	}
	else
	{
		mValues.erase(key);
		return;
	}
}

template <typename TValue>
bool aiva::layer1::ShaderStruct::GetArray(std::string const& key, std::vector<TValue> *const value) const
{
	aiva::utils::Asserts::CheckBool(value);

	auto const& arrayIter = mValues.find(key);
	if (arrayIter == mValues.end())
	{
		*value = {};
		return false;
	}

	auto const& basicArray = arrayIter->second;
	aiva::utils::Asserts::CheckBool(basicArray);

	auto const& specificArray = std::dynamic_pointer_cast<TShaderArray<TValue>>(basicArray);
	aiva::utils::Asserts::CheckBool(specificArray);

	*value = specificArray->Value();
	return true;
}

template <typename TValue>
void aiva::layer1::ShaderStruct::SetArray(std::string const& key, std::vector<TValue> const*const value)
{
	if (value)
	{
		auto const& specificArray = TShaderArray<TValue>::Create(*value);
		aiva::utils::Asserts::CheckBool(specificArray);

		mValues.insert_or_assign(key, specificArray);
		return;
	}
	else
	{
		mValues.erase(key);
		return;
	}
}
