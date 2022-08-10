#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/e_boxed_type.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::utils
{
	struct MetaField;
}

namespace aiva::utils
{
	struct MetaStruct final : public AObject, public IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		MetaStruct();

	public:
		~MetaStruct() override;

	// ----------------------------------------------------
	// Metadata

	public:
		using FieldElementType = MetaField;

		using FieldPointerType = std::shared_ptr<FieldElementType>;

		using FieldPointerArrayType = std::vector<FieldPointerType>;

	public:
		MetaStruct& Add(FieldPointerType const& field);

		FieldPointerType const& Get(std::size_t index) const;

		std::size_t Num() const;

	private:
		void Field_OnChanged();

	private:
		FieldPointerArrayType mFields{};
	};
}
