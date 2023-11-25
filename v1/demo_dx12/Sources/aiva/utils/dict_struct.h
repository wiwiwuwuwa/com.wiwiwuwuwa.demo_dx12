#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::utils
{
	struct LayoutStruct;
	struct IBoxedValue;
	template <typename> struct TBoxedValue;
}

namespace aiva::utils
{
	struct DictStruct final : public AObject, public IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		DictStruct();

	public:
		~DictStruct() override;

	// ----------------------------------------------------
	// Fields

	public:
		using FieldElemType = std::shared_ptr<IBoxedValue>;

		using FieldDictType = std::unordered_map<std::string, FieldElemType>;

	// --------------------------------

	public:
		FieldElemType FieldBoxed(std::string const& name) const;

		DictStruct& FieldBoxed(std::string const& name, FieldElemType const& fieldBoxed);

		FieldDictType const& FieldsBoxed() const;

	private:
		void FieldBoxed_OnChanged();

	private:
		FieldDictType mFieldsBoxed{};

	// --------------------------------

	public:
		template <typename TValue>
		using TFieldElemType = std::shared_ptr<TBoxedValue<TValue>>;

	public:
		template <typename TValue>
		TFieldElemType<TValue> FieldBoxed(std::string const& name) const;

		template <typename TValue>
		DictStruct& FieldBoxed(std::string const& name, TFieldElemType<TValue> const& fieldBoxed);

	// --------------------------------

	public:
		template <typename TValue>
		std::optional<TValue> FieldOptional(std::string const& name) const;

		template <typename TValue>
		DictStruct& FieldOptional(std::string const& name, std::optional<TValue> const& field);

	// --------------------------------

	public:
		template <typename TValue>
		TValue FieldValue(std::string const& name) const;

		template <typename TValue>
		DictStruct& FieldValue(std::string const& name, TValue const& field);
	};
}

// --------------------------------------------------------

#include <aiva/utils/asserts.h>
#include <aiva/utils/boxed_value_utils.h>
#include <aiva/utils/i_boxed_value.h>
#include <aiva/utils/t_boxed_value.h>

template <typename TValue>
aiva::utils::DictStruct::TFieldElemType<TValue> aiva::utils::DictStruct::FieldBoxed(std::string const& name) const
{
	auto const basicField = FieldBoxed(name);
	if (!basicField) return {};

	auto const specificField = std::dynamic_pointer_cast<TFieldElemType<TValue>>(basicField);
	Asserts::CheckBool(specificField, "Specific field is not valid");

	return specificField;
}

template <typename TValue>
aiva::utils::DictStruct& aiva::utils::DictStruct::FieldBoxed(std::string const& name, TFieldElemType<TValue> const& fieldBoxed)
{
	auto const basicField = std::static_pointer_cast<FieldElemType::element_type>(fieldBoxed);
	return FieldBoxed(name, basicField);
}

template <typename TValue>
std::optional<TValue> aiva::utils::DictStruct::FieldOptional(std::string const& name) const
{
	auto const fieldBoxed = FieldBoxed<TValue>(name);
	if (!fieldBoxed) return {};

	return fieldBoxed->Value();
}

template <typename TValue>
aiva::utils::DictStruct& aiva::utils::DictStruct::FieldOptional(std::string const& name, std::optional<TValue> const& field)
{
	if (field)
	{
		auto const fieldBoxed = BoxedValueUtils::CreateInstance<TValue>();
		fieldBoxed->Value(field.value());

		return FieldBoxed(name, fieldBoxed);
	}
	else
	{
		return FieldBoxed(name, {});
	}
}

template <typename TValue>
TValue aiva::utils::DictStruct::FieldValue(std::string const& name) const
{
	auto const fieldOptional = FieldOptional<TValue>(name);
	Asserts::CheckBool(fieldOptional, "Field optional is not valid");

	return fieldOptional.value();
}

template <typename TValue>
aiva::utils::DictStruct& aiva::utils::DictStruct::FieldValue(std::string const& name, TValue const& field)
{
	auto const fieldOptional = std::optional<TValue>{ field };
	return FieldOptional(name, fieldOptional);
}
