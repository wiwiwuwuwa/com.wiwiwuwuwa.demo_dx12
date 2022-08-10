#include <pch.h>
#include <aiva/utils/meta_field.h>

aiva::utils::MetaField::MetaField() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::MetaField::~MetaField()
{

}

std::string const& aiva::utils::MetaField::Name() const
{
	return mName;
}

aiva::utils::MetaField& aiva::utils::MetaField::Name(std::string const& name)
{
	if (mName != name)
	{
		mName = name;
		OnChanged()();
	}

	return *this;
}

aiva::utils::MetaField::TypeEnum aiva::utils::MetaField::Type() const
{
	return mType;
}

aiva::utils::MetaField& aiva::utils::MetaField::Type(TypeEnum const type)
{
	if (mType != type)
	{
		mType = type;
		OnChanged()();
	}

	return *this;
}
