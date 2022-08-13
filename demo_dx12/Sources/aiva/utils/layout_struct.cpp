#include <pch.h>
#include <aiva/utils/layout_struct.h>

#include <aiva/utils/layout_field.h>
#include <aiva/utils/asserts.h>

aiva::utils::LayoutStruct::LayoutStruct() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::LayoutStruct::~LayoutStruct()
{

}

std::size_t aiva::utils::LayoutStruct::Offset() const
{
	return mOffset;
}

aiva::utils::LayoutStruct& aiva::utils::LayoutStruct::Offset(std::size_t const offset)
{
	if (mOffset != offset)
	{
		mOffset = offset;
		BroadcastCacheDataChanged();
	}

	return *this;
}

std::size_t aiva::utils::LayoutStruct::Size() const
{
	return mSize;
}

aiva::utils::LayoutStruct& aiva::utils::LayoutStruct::Size(std::size_t const size)
{
	if (mSize != size)
	{
		mSize = size;
		BroadcastCacheDataChanged();
	}

	return *this;
}

aiva::utils::LayoutStruct::FieldElemType aiva::utils::LayoutStruct::Field(std::string const& name) const
{
	auto const iter = mFields.find(name);
	if (iter == mFields.end()) return {};

	auto const& field = iter->second;
	Asserts::CheckBool(field, "Field is not valid");

	return field;
}

aiva::utils::LayoutStruct& aiva::utils::LayoutStruct::Field(std::string const& name, FieldElemType const& field)
{
	auto const previousField = Field(name);
	if (previousField)
	{
		previousField->OnCacheDataChanged().disconnect(boost::bind(&LayoutStruct::Field_OnChanged, this));
		mFields.erase(name);
	}

	auto const& desiredField = field;
	if (desiredField)
	{
		desiredField->OnCacheDataChanged().connect(boost::bind(&LayoutStruct::Field_OnChanged, this));
		mFields.insert_or_assign(name, desiredField);
	}

	BroadcastCacheDataChanged();
	return *this;
}

aiva::utils::LayoutStruct::FieldDictType const& aiva::utils::LayoutStruct::Fields() const
{
	return mFields;
}

void aiva::utils::LayoutStruct::Field_OnChanged()
{
	BroadcastCacheDataChanged();
}
