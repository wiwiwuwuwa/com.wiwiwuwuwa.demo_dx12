#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/e_boxed_type.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::utils
{
	struct MetaField final : public AObject, public IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		MetaField();

	public:
		~MetaField() override;

	// ----------------------------------------------------
	// Metadata

	public:
		std::string const& Name() const;

		MetaField& Name(std::string const& name);

	private:
		std::string mName{};

	// --------------------------------

	public:
		using TypeEnum = EBoxedType;

	public:
		TypeEnum Type() const;

		MetaField& Type(TypeEnum const type);

	private:
		TypeEnum mType{ TypeEnum::UNKNOWN };
	};
}
