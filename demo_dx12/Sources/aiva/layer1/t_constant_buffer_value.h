#pragma once
#include <pch.h>

#include <aiva/layer1/i_constant_buffer_value.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/changes_counter.h>

namespace aiva::layer1
{
	template <typename TValue>
	struct TConstantBufferValue : private boost::noncopyable, public std::enable_shared_from_this<TConstantBufferValue<TValue>>, public aiva::layer1::IConstantBufferValue
	{
		// ------------------------------------------------
		// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<aiva::layer1::TConstantBufferValue<TValue>> Create(TArgs&&... args);

	private:
		TConstantBufferValue();

	public:
		~TConstantBufferValue();

		// ------------------------------------------------
		// IConstantBufferValue

	public:
		boost::span<const std::byte> GetRawData() const override;

		// ------------------------------------------------
		// High-Level Data

	public:
		TValue const& Value() const;

		TConstantBufferValue<TValue> const& Value(TValue const& value);

	private:
		TValue mValue{};
	};
}

// --------------------------------------------------------

template <typename TValue>
template <typename... TArgs>
std::shared_ptr<aiva::layer1::TConstantBufferValue<TValue>> aiva::layer1::TConstantBufferValue<TValue>::Create(TArgs&&... args)
{
	return std::shared_ptr<aiva::layer1::TConstantBufferValue<TValue>>{new aiva::layer1::TConstantBufferValue<TValue>{ std::forward<TArgs>(args)... }};
}

template <typename TValue>
aiva::layer1::TConstantBufferValue<TValue>::TConstantBufferValue()
{

}

template <typename TValue>
aiva::layer1::TConstantBufferValue<TValue>::~TConstantBufferValue()
{

}

template <typename TValue>
boost::span<const std::byte> aiva::layer1::TConstantBufferValue<TValue>::GetRawData() const
{
	return boost::as_bytes(boost::span<const TValue>{std::addressof(mValue), std::size_t{ 1 }});
}

template <typename TValue>
TValue const& aiva::layer1::TConstantBufferValue<TValue>::Value() const
{
	return mValue;
}

template <typename TValue>
aiva::layer1::TConstantBufferValue<TValue> const& aiva::layer1::TConstantBufferValue<TValue>::Value(TValue const& value)
{
	mValue = value;
	return *this;
}
