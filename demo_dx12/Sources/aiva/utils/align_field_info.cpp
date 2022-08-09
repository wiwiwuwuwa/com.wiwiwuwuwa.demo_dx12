#include <pch.h>
#include <aiva/utils/align_field_info.h>

aiva::utils::AlignFieldInfo::AlignFieldInfo() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::AlignFieldInfo::~AlignFieldInfo()
{

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
