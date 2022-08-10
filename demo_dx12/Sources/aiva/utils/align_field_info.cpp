#include <pch.h>
#include <aiva/utils/align_field_info.h>

aiva::utils::AlignFieldInfo::AlignFieldInfo() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::AlignFieldInfo::~AlignFieldInfo()
{

}

aiva::utils::AlignFieldInfo::TypeEnum aiva::utils::AlignFieldInfo::Type() const
{
	return mType;
}

aiva::utils::AlignFieldInfo& aiva::utils::AlignFieldInfo::Type(TypeEnum const type)
{
	if (mType != type)
	{
		mType = type;
		OnChanged()();
	}

	return *this;
}

std::size_t aiva::utils::AlignFieldInfo::Offset() const
{
	return mOffset;
}

aiva::utils::AlignFieldInfo& aiva::utils::AlignFieldInfo::Offset(std::size_t const offset)
{
	if (mOffset != offset)
	{
		mOffset = offset;
		OnChanged()();
	}

	return *this;
}

std::size_t aiva::utils::AlignFieldInfo::Size() const
{
	return mSize;
}

aiva::utils::AlignFieldInfo& aiva::utils::AlignFieldInfo::Size(std::size_t const size)
{
	if (mSize != size)
	{
		mSize = size;
		OnChanged()();
	}

	return *this;
}
