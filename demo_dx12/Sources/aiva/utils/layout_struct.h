#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::utils
{
	struct LayoutField;
}

namespace aiva::utils
{
	struct LayoutStruct final : public AObject, public IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		LayoutStruct();

	public:
		~LayoutStruct() override;

	// ----------------------------------------------------
	// Metadata

	public:
		std::size_t Offset() const;

		LayoutStruct& Offset(std::size_t const offset);

	private:
		std::size_t mOffset{};

	// --------------------------------

	public:
		std::size_t Size() const;

		LayoutStruct& Size(std::size_t const size);

	private:
		std::size_t mSize{};

	// ----------------------------------------------------
	// Fields

	public:
		using FieldElemType = std::shared_ptr<LayoutField>;

		using FieldDictType = std::unordered_map<std::string, FieldElemType>;

	public:
		FieldElemType Field(std::string const& name) const;

		LayoutStruct& Field(std::string const& name, FieldElemType const& field);

		FieldDictType const& Fields() const;

	private:
		void Field_OnChanged();

	private:
		FieldDictType mFields{};
	};
}
