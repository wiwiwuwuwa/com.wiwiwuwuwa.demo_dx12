#pragma once
#include <pch.h>

namespace aiva::utils
{
	enum class EBoxedType;
	struct IBoxedValue;
	template <typename> struct TBoxedValue;
}

namespace aiva::utils
{
	struct BoxedValueUtils final
	{
	// ----------------------------------------------------
	// Main

	private:
		BoxedValueUtils();

		~BoxedValueUtils();

	// ----------------------------------------------------
	// Instance

	private:
		static BoxedValueUtils& Instance();

	// ----------------------------------------------------
	// Type Mapper

	private:
		void InitializeTypeMapper();

		void TerminateTypeMapper();

	private:
		template <typename TValue>
		void RegisterTypeMapper(EBoxedType const boxedType);

	// ----------------------------------------------------
	// TypeOf

	public:
		static EBoxedType TypeOf(std::shared_ptr<IBoxedValue> const& boxedValue);

		template <typename TValue>
		static EBoxedType TypeOf();

	private:
		using TypeOfDictType = std::unordered_map<std::type_index, EBoxedType>;

	private:
		template <typename TValue>
		void RegisterTypeOfElem(EBoxedType const boxedType);

	private:
		TypeOfDictType mTypeOfDict{};

	// ----------------------------------------------------
	// CreateInstance

	public:
		static std::shared_ptr<IBoxedValue> CreateInstance(EBoxedType const boxedType);

		template <typename TValue>
		static std::shared_ptr<TBoxedValue<TValue>> CreateInstance();

	private:
		using CreateInstanceElemType = std::function<std::shared_ptr<IBoxedValue>()>;

		using CreateInstanceDictType = std::unordered_map<EBoxedType, CreateInstanceElemType>;

	private:
		template <typename TValue>
		void RegisterCreateInstanceElem(EBoxedType const boxedType);

	private:
		CreateInstanceDictType mCreateInstanceDict{};

	// ----------------------------------------------------
	// CastTo

	public:
		static std::shared_ptr<IBoxedValue> CastTo(std::shared_ptr<IBoxedValue> const& boxedValue, EBoxedType const boxedType);

		template <typename TValue>
		static std::shared_ptr<TBoxedValue<TValue>> CastTo(std::shared_ptr<IBoxedValue> const& boxedValue);

	// ----------------------------------------------------
	// SerializeToBinary

	public:
		static std::vector<std::byte> SerializeToBinary(std::shared_ptr<IBoxedValue> const& boxedValue);

	// ----------------------------------------------------
	// DeserealizeFromBinary

	public:
		static void DeserealizeFromBinary(std::shared_ptr<IBoxedValue> const& boxedValue, boost::span<const std::byte> const& binary);

		static std::shared_ptr<IBoxedValue> DeserealizeFromBinary(boost::span<const std::byte> const& binary, EBoxedType const boxedType);

		template <typename TValue>
		static std::shared_ptr<TBoxedValue<TValue>> DeserealizeFromBinary(boost::span<const std::byte> const& binary);
	};
}
