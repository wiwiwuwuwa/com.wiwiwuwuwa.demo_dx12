#include <pch.h>
#include <aiva/utils/layout_field.h>

aiva::utils::LayoutField::LayoutField() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::LayoutField::~LayoutField()
{

}

aiva::utils::LayoutField::TypeEnum aiva::utils::LayoutField::Type() const
{
	return mType;
}

aiva::utils::LayoutField& aiva::utils::LayoutField::Type(TypeEnum const type)
{
	if (mType != type)
	{
		mType = type;
		BroadcastCacheDataChanged();
	}

	return *this;
}

std::size_t aiva::utils::LayoutField::Offset() const
{
	return mOffset;
}

aiva::utils::LayoutField& aiva::utils::LayoutField::Offset(std::size_t const offset)
{
	if (mOffset != offset)
	{
		mOffset = offset;
		BroadcastCacheDataChanged();
	}

	return *this;
}

std::size_t aiva::utils::LayoutField::Size() const
{
	return mSize;
}

aiva::utils::LayoutField& aiva::utils::LayoutField::Size(std::size_t const size)
{
	if (mSize != size)
	{
		mSize = size;
		BroadcastCacheDataChanged();
	}

	return *this;
}
