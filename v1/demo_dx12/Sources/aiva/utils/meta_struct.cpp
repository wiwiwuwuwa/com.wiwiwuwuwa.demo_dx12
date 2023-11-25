#include <pch.h>
#include <aiva/utils/meta_struct.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/meta_field.h>

aiva::utils::MetaStruct::MetaStruct() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::MetaStruct::~MetaStruct()
{

}

aiva::utils::MetaStruct& aiva::utils::MetaStruct::Add(FieldPointerType const& field)
{
	Asserts::CheckBool(field, "Field is not valid");

	field->OnCacheDataChanged().connect(boost::bind(&MetaStruct::Field_OnChanged, this));
	mFields.emplace_back(field);

	BroadcastCacheDataChanged();
	return *this;
}

aiva::utils::MetaStruct::FieldPointerType const& aiva::utils::MetaStruct::Get(std::size_t index) const
{
	return mFields.at(index);
}

std::size_t aiva::utils::MetaStruct::Num() const
{
	return std::size(mFields);
}

void aiva::utils::MetaStruct::Field_OnChanged()
{
	BroadcastCacheDataChanged();
}
