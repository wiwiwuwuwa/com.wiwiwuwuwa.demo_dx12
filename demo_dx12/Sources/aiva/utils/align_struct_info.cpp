#include <pch.h>
#include <aiva/utils/align_struct_info.h>

#include <aiva/utils/align_field_info.h>
#include <aiva/utils/asserts.h>

aiva::utils::AlignStructInfo::AlignStructInfo() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::AlignStructInfo::~AlignStructInfo()
{

}

std::size_t aiva::utils::AlignStructInfo::Size() const
{
	return mSize;
}

aiva::utils::AlignStructInfo& aiva::utils::AlignStructInfo::Size(std::size_t const size)
{
	if (mSize != size)
	{
		mSize = size;
		OnChanged()();
	}

	return *this;
}

aiva::utils::AlignStructInfo::FieldElemType aiva::utils::AlignStructInfo::Field(std::string const& name) const
{
	auto const iter = mFields.find(name);
	if (iter == mFields.end()) return {};

	auto const& field = iter->second;
	Asserts::CheckBool(field, "Field is not valid");

	return field;
}

aiva::utils::AlignStructInfo& aiva::utils::AlignStructInfo::Field(std::string const& name, FieldElemType const& field)
{
	auto const previousField = Field(name);
	if (previousField)
	{
		previousField->OnChanged().disconnect(boost::bind(&AlignStructInfo::Field_OnChanged, this));
		mFields.erase(name);
	}

	auto const& desiredField = field;
	if (desiredField)
	{
		desiredField->OnChanged().connect(boost::bind(&AlignStructInfo::Field_OnChanged, this));
		mFields.insert_or_assign(name, desiredField);
	}

	OnChanged()();
	return *this;
}

aiva::utils::AlignStructInfo::FieldDictType const& aiva::utils::AlignStructInfo::Fields() const
{
	return mFields;
}

void aiva::utils::AlignStructInfo::Field_OnChanged()
{
	OnChanged()();
}
