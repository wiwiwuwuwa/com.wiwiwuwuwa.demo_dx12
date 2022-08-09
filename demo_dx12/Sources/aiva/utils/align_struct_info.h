#pragma once
#include <pch.h>

#include <aiva/utils/a_object.h>
#include <aiva/utils/i_object_changeable.h>

namespace aiva::utils
{
	struct AlignFieldInfo;
}

namespace aiva::utils
{
	struct AlignStructInfo final : public AObject, public IObjectChangeable
	{
	// ----------------------------------------------------
	// Main

	private:
		friend FactoryType;

	protected:
		AlignStructInfo();

	public:
		~AlignStructInfo() override;

	// ----------------------------------------------------
	// Metadata

	public:
		std::size_t Size() const;

		AlignStructInfo& Size(std::size_t const size);

	private:
		std::size_t mSize{};

	// ----------------------------------------------------
	// Fields

	public:
		using FieldElemType = std::shared_ptr<AlignFieldInfo>;

		using FieldDictType = std::unordered_map<std::string, FieldElemType>;

	public:
		FieldElemType Field(std::string const& name) const;

		AlignStructInfo& Field(std::string const& name, FieldElemType const& field);

		FieldDictType const& Fields() const;

	private:
		void Field_OnChanged();

	private:
		FieldDictType mFields{};
	};
}
