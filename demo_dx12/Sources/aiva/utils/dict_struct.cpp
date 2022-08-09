#include <pch.h>
#include <aiva/utils/dict_struct.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/i_boxed_value.h>

aiva::utils::DictStruct::DictStruct() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::DictStruct::~DictStruct()
{

}

aiva::utils::DictStruct::FieldElemType aiva::utils::DictStruct::FieldBoxed(std::string const& name) const
{
	auto const iter = mFieldsBoxed.find(name);
	if (iter == mFieldsBoxed.end()) return {};

	auto const& field = iter->second;
	Asserts::CheckBool(field, "Field is not valid");

	return field;
}

aiva::utils::DictStruct& aiva::utils::DictStruct::FieldBoxed(std::string const& name, FieldElemType const& fieldBoxed)
{
	auto const previousField = FieldBoxed(name);
	if (previousField)
	{
		previousField->OnChanged().disconnect(boost::bind(&DictStruct::FieldBoxed_OnChanged, this));
		mFieldsBoxed.erase(name);
	}

	auto const& desiredField = fieldBoxed;
	if (desiredField)
	{
		desiredField->OnChanged().connect(boost::bind(&DictStruct::FieldBoxed_OnChanged, this));
		mFieldsBoxed.insert_or_assign(name, desiredField);
	}

	OnChanged()();
	return *this;
}

void aiva::utils::DictStruct::FieldBoxed_OnChanged()
{
	OnChanged()();
}
