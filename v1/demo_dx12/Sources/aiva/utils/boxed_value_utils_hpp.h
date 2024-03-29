#pragma once
#include <pch.h>

namespace aiva::utils
{
	enum class EBoxedType;
	struct IBoxedValue;
	struct GltfTypeDesc;
	struct MetaField;
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
	// SizeOf

	public:
		static std::size_t SizeOf(EBoxedType const boxedType);

	private:
		using SizeOfElemType = std::function<std::size_t()>;

		using SizeOfDictType = std::unordered_map<EBoxedType, SizeOfElemType>;

	private:
		template <typename TValue>
		void RegisterSizeOfElem(EBoxedType const boxedType);

	private:
		SizeOfDictType mSizeOfDict{};

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
		static std::shared_ptr<IBoxedValue> CreateInstance(GltfTypeDesc const& desc);

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
	// Comparison

	public:
		using MetaFieldElementType = MetaField;

		using MetaFieldPointerType = std::shared_ptr<MetaField>;

	public:
		static bool IsMatchingLayout(std::shared_ptr<IBoxedValue> const& dstField, MetaFieldPointerType const& srcField);

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
