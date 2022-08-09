#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/i_boxed_value.h>

namespace aiva::utils
{
	enum class EBoxedType;
}

namespace aiva::utils
{
	template <typename TValue>
	struct TBoxedValue final : public AObject, public IBoxedValue
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	public:
		using ValueType = TValue;

	protected:
		TBoxedValue(ValueType const& value = {});

	public:
		~TBoxedValue() override;

	// ----------------------------------------------------
	// Type

	public:
		EBoxedType Type() const override;

	// ----------------------------------------------------
	// Value

	public:
		ValueType const& Value() const;

		TBoxedValue& Value(ValueType const& value);

	private:
		ValueType mValue{};

	// ----------------------------------------------------
	// Binary

	public:
		boost::span<std::byte> Binary() const override;
	};
}
