#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/e_boxed_type.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::utils
{
	struct LayoutField final : public AObject, public IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		LayoutField();

	public:
		~LayoutField() override;

	// ----------------------------------------------------
	// Metadata

	public:
		using TypeEnum = EBoxedType;

	public:
		TypeEnum Type() const;

		LayoutField& Type(TypeEnum const type);

	private:
		TypeEnum mType{ TypeEnum::UNKNOWN };

	// --------------------------------

	public:
		std::size_t Offset() const;

		LayoutField& Offset(std::size_t const offset);

	private:
		std::size_t mOffset{};

	// --------------------------------

	public:
		std::size_t Size() const;

		LayoutField& Size(std::size_t const size);

	private:
		std::size_t mSize{};
	};
}
