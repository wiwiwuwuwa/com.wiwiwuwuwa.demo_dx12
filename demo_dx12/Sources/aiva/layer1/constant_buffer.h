#pragma once
#include <pch.h>

#include <aiva/layer1/t_constant_buffer_value.h>
#include <aiva/utils/asserts.h>
#include <aiva/utils/enum_utils.h>
#include <aiva/utils/t_changes_detector.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct ConstantBuffer final : private boost::noncopyable, public std::enable_shared_from_this<ConstantBuffer>
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<aiva::layer1::ConstantBuffer> Create(TArgs&&... args);

	private:
		ConstantBuffer(aiva::layer1::Engine const& engine);

	public:
		~ConstantBuffer();

	private:
		aiva::layer1::Engine const& mEngine;

	// ----------------------------------------------------
	// Changes Detection

	public:
		enum class EDirtyFlags : std::uint8_t
		{
			None = 0,
			All = 1,
		};

	public:
		aiva::layer1::ConstantBuffer& MarkAsChanged(EDirtyFlags const dirtyFlags = EDirtyFlags::All);

		aiva::layer1::ConstantBuffer& FlushChanges();

	private:
		aiva::utils::TChangesDetector<EDirtyFlags> mChangesDetector{};

	// ----------------------------------------------------
	// High-Level Data

	public:
		template <typename TValue>
		TValue const& GetValue(std::string const& key) const;

		template <typename TValue>
		aiva::layer1::ConstantBuffer& SetValue(std::string const& key, TValue const& value);

	private:
		std::unordered_map<std::string, std::shared_ptr<aiva::layer1::IConstantBufferValue>> mValues{};

	// ----------------------------------------------------
	// Low-Level Data

	public:
		winrt::com_ptr<ID3D12Resource> const& RawResource();

	private:
		void InitializeLowLevelData();

		void TerminateLowLevelData();

	private:
		void RefreshLowLevelData(EDirtyFlags const dirtyFlags);

		void RefreshResourceObject(boost::span<const std::byte> const& binaryData);

		void RefreshResourceData(boost::span<const std::byte> const& binaryData);

	private:
		winrt::com_ptr<ID3D12Resource> mRawResource{};
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::ConstantBuffer> aiva::layer1::ConstantBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<aiva::layer1::ConstantBuffer>{new aiva::layer1::ConstantBuffer{ std::forward<TArgs>(args)... }};
}

template <typename TValue>
TValue const& aiva::layer1::ConstantBuffer::GetValue(std::string const& key) const
{
	aiva::utils::Asserts::CheckBool(!key.empty());

	auto const& valueIter = mValues.find(key);
	aiva::utils::Asserts::CheckBool(valueIter != mValues.end());

	auto const& valueBasicPointer = valueIter->second;
	aiva::utils::Asserts::CheckBool(valueBasicPointer);

	auto const& valueSpecificPointer = std::dynamic_pointer_cast<aiva::layer1::TConstantBufferValue<TValue>>(valueBasicPointer);
	aiva::utils::Asserts::CheckBool(valueSpecificPointer);

	return valueSpecificPointer->Value();
}

template <typename TValue>
aiva::layer1::ConstantBuffer& aiva::layer1::ConstantBuffer::SetValue(std::string const& key, TValue const& value)
{
	aiva::utils::Asserts::CheckBool(!key.empty());

	auto const& valuePointer = aiva::layer1::TConstantBufferValue<TValue>::Create(value);
	mValues.insert_or_assign(key, valuePointer);

	MarkAsChanged(EDirtyFlags::All);
	return *this;
}
