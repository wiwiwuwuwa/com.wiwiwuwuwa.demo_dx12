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
		std::vector<std::byte> const& GetRawData() const override;

		// ------------------------------------------------
		// Changes Detection

	private:
		TConstantBufferValue<TValue>& BeginChanges();

		TConstantBufferValue<TValue>& EndChanges();

	private:
		aiva::utils::ChangesCounter mChangesCounter{};

		// ------------------------------------------------
		// High-Level Data

	public:
		TValue const& Value() const;

		TConstantBufferValue<TValue> const& Value(TValue const& value);

	private:
		TValue mValue{};

		// ------------------------------------------------
		// Low-Level Data

	private:
		void InitializeLowLevelData();

		void TerminateLowLevelData();

	private:
		void RefreshLowLevelData();

	private:
		std::vector<std::byte> mRawData{};
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
	InitializeLowLevelData();
}

template <typename TValue>
aiva::layer1::TConstantBufferValue<TValue>::~TConstantBufferValue()
{
	TerminateLowLevelData();
}

template <typename TValue>
std::vector<std::byte> const& aiva::layer1::TConstantBufferValue<TValue>::GetRawData() const
{
	aiva::utils::Asserts::CheckBool(!mRawData.empty());
	return mRawData;
}

template <typename TValue>
aiva::layer1::TConstantBufferValue<TValue>& aiva::layer1::TConstantBufferValue<TValue>::BeginChanges()
{
	mChangesCounter.IncrementChanges();
	return *this;
}

template <typename TValue>
aiva::layer1::TConstantBufferValue<TValue>& aiva::layer1::TConstantBufferValue<TValue>::EndChanges()
{
	mChangesCounter.DecrementChanges();
	return *this;
}

template <typename TValue>
TValue const& aiva::layer1::TConstantBufferValue<TValue>::Value() const
{
	return mValue;
}

template <typename TValue>
aiva::layer1::TConstantBufferValue<TValue> const& aiva::layer1::TConstantBufferValue<TValue>::Value(TValue const& value)
{
	BeginChanges();
	mValue = value;
	EndChanges();

	return *this;
}

template <typename TValue>
void aiva::layer1::TConstantBufferValue<TValue>::InitializeLowLevelData()
{
	mChangesCounter.OnChangesFinished().connect(boost::bind(&aiva::layer1::TConstantBufferValue<TValue>::RefreshLowLevelData, this));
	RefreshLowLevelData();
}

template <typename TValue>
void aiva::layer1::TConstantBufferValue<TValue>::TerminateLowLevelData()
{
	mChangesCounter.OnChangesFinished().disconnect(boost::bind(&aiva::layer1::TConstantBufferValue<TValue>::RefreshLowLevelData, this));
}

template <typename TValue>
void aiva::layer1::TConstantBufferValue<TValue>::RefreshLowLevelData()
{
	std::vector<std::byte> rawData{ sizeof(TValue) };
	*reinterpret_cast<TValue*>(rawData.data()) = Value();

	mRawData = rawData;
}
