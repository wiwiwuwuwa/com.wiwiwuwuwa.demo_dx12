#pragma once
#include <pch.h>

#include <aiva/layer1/i_shader_value.h>
#include <aiva/layer1/t_shader_array.h>
#include <aiva/layer1/t_shader_value.h>
#include <aiva/utils/a_object.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::layer1
{
	struct ShaderStruct final : public aiva::utils::AObject, public aiva::utils::IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		ShaderStruct();

	public:
		~ShaderStruct() override;

	// ----------------------------------------------------
	// Serialization

	public:
		std::vector<std::byte> SerializeToBinary() const;

	// ----------------------------------------------------
	// Data

	public:
		using MapType = std::unordered_map<std::string, std::shared_ptr<IShaderValue>>;

	public:
		template <typename TValue>
		bool GetValue(std::string const& key, TValue *const value) const;

		template <typename TValue>
		void SetValue(std::string const& key, TValue const*const value);

		template <typename TValue>
		bool GetArray(std::string const& key, std::vector<TValue> *const value) const;

		template <typename TValue>
		void SetArray(std::string const& key, std::vector<TValue> const*const value);

		template <typename TValue>
		bool GetStruct(std::string const& key, std::shared_ptr<TShaderValue<TValue>> *const value) const;

		template <typename TValue>
		void SetStruct(std::string const& key, std::shared_ptr<TShaderValue<TValue>> const*const value) const;

	public:
		bool GetStruct(std::string const& key, std::shared_ptr<IShaderValue> *const value) const;

		void SetStruct(std::string const& key, std::shared_ptr<IShaderValue> const*const value);

		std::size_t Num() const;

		bool HasSameFields(ShaderStruct const& other) const;

	private:
		MapType mValues{};
	};
}

// --------------------------------------------------------

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
		auto const& specificValue = TShaderValue<TValue>::FactoryType::Create<TShaderValue<TValue>>(*value);
		aiva::utils::Asserts::CheckBool(specificValue);

		mValues.insert_or_assign(key, specificValue);
	}
	else
	{
		mValues.erase(key);
	}

	OnChanged()();
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
	}
	else
	{
		mValues.erase(key);
	}

	OnChanged()();
}

template <typename TValue>
bool aiva::layer1::ShaderStruct::GetStruct(std::string const& key, std::shared_ptr<TShaderValue<TValue>> *const value) const
{
	aiva::utils::Asserts::CheckBool(value);

	auto basicValue = std::shared_ptr<IShaderValue>{};
	if (!GetStruct(key, &basicValue))
	{
		*value = {};
		return false;
	}

	auto const& specificValue = std::dynamic_pointer_cast<TShaderValue<TValue>>(basicValue);
	aiva::utils::Asserts::CheckBool(specificValue);

	*value = specificValue;
	return true;
}

template <typename TValue>
void aiva::layer1::ShaderStruct::SetStruct(std::string const& key, std::shared_ptr<TShaderValue<TValue>> const*const value) const
{
	SetStruct(key, value);
}
