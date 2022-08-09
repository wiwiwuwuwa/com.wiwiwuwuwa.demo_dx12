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
	// TypeOf

	public:
		static EBoxedType TypeOf(std::shared_ptr<IBoxedValue> const& boxedValue);

		template <typename TValue>
		static EBoxedType TypeOf();

	// ----------------------------------------------------
	// CreateInstance

	public:
		static std::shared_ptr<IBoxedValue> CreateInstance(EBoxedType const boxedType);

		template <typename TValue>
		static std::shared_ptr<TBoxedValue<TValue>> CreateInstance();

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
