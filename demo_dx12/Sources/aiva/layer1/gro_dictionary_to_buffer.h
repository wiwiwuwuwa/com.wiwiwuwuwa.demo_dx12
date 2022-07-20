#pragma once
#include <pch.h>

#include <aiva/layer1/gro_dictionary_to_buffer_desc.h>
#include <aiva/layer1/i_gpu_resource_operator.h>
#include <aiva/layer1/t_constant_buffer_value.h>
#include <aiva/utils/action_batcher.h>

namespace aiva::layer1
{
	struct Engine;
}

namespace aiva::layer1
{
	struct GroDictionaryToBuffer final : private boost::noncopyable, public std::enable_shared_from_this<GroDictionaryToBuffer>, public IGpuResourceOperator
	{
	// ----------------------------------------------------
	// Main

	public:
		template <typename... TArgs>
		static std::shared_ptr<GroDictionaryToBuffer> Create(TArgs&&... args);

	private:
		GroDictionaryToBuffer(Engine const& engine, GroDictionaryToBufferDesc const& desc);

	public:
		~GroDictionaryToBuffer();

	private:
		Engine const& mEngine;

	// ----------------------------------------------------
	// Descriptor

	public:
		GroDictionaryToBufferDesc Desc() const;

		GroDictionaryToBuffer& Desc(GroDictionaryToBufferDesc const& desc);

	private:
		GroDictionaryToBufferDesc mDesc{};

	// ----------------------------------------------------
	// Batcher

	public:
		aiva::utils::ActionBatcher& GetBatcher();

	private:
		aiva::utils::ActionBatcher mBatcher{};

	// ----------------------------------------------------
	// Dictionary

	public:
		template <typename TValue>
		TValue const& GetValue(std::string const& key) const;

		template <typename TValue>
		GroDictionaryToBuffer& SetValue(std::string const& key, TValue const& value);

	private:
		std::unordered_map<std::string, std::shared_ptr<IConstantBufferValue>> mValues{};

	// ----------------------------------------------------
	// Updated

	private:
		void InitializeUpdater();

		void TerminateUpdater();

	private:
		void UpdateInternalResources();
	};
}

// --------------------------------------------------------

template <typename... TArgs>
std::shared_ptr<aiva::layer1::GroDictionaryToBuffer> aiva::layer1::GroDictionaryToBuffer::Create(TArgs&&... args)
{
	return std::shared_ptr<GroDictionaryToBuffer>{new GroDictionaryToBuffer{ std::forward<TArgs>(args)... }};
}

template <typename TValue>
TValue const& aiva::layer1::GroDictionaryToBuffer::GetValue(std::string const& key) const
{
	aiva::utils::Asserts::CheckBool(!key.empty());

	auto const& valueIter = mValues.find(key);
	aiva::utils::Asserts::CheckBool(valueIter != mValues.end());

	auto const& valueBasicPointer = valueIter->second;
	aiva::utils::Asserts::CheckBool(valueBasicPointer);

	auto const& valueSpecificPointer = std::dynamic_pointer_cast<TConstantBufferValue<TValue>>(valueBasicPointer);
	aiva::utils::Asserts::CheckBool(valueSpecificPointer);

	return valueSpecificPointer->Value();
}

template <typename TValue>
aiva::layer1::GroDictionaryToBuffer& aiva::layer1::GroDictionaryToBuffer::SetValue(std::string const& key, TValue const& value)
{
	aiva::utils::Asserts::CheckBool(!key.empty());
	
	GetBatcher().BeginBatch();
	auto const& valuePointer = TConstantBufferValue<TValue>::Create(value);
	mValues.insert_or_assign(key, valuePointer);
	GetBatcher().EndBatch();
	
	return *this;
}
