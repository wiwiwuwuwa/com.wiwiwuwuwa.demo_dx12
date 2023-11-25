#include <pch.h>
#include <aiva/utils/dict_buffer.h>

#include <aiva/utils/asserts.h>
#include <aiva/utils/dict_struct.h>
#include <aiva/utils/dict_struct_utils.h>
#include <aiva/utils/layout_struct.h>
#include <aiva/utils/layout_struct_utils.h>
#include <aiva/utils/meta_struct.h>
#include <aiva/utils/meta_struct_utils.h>

aiva::utils::DictBuffer::DictBuffer() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::DictBuffer::~DictBuffer()
{

}

aiva::utils::DictBuffer::MetaStructPointerType const& aiva::utils::DictBuffer::Layout() const
{
	return mLayout;
}

aiva::utils::DictBuffer& aiva::utils::DictBuffer::Layout(DictStructPointerType const& layout)
{
	if (layout)
	{
		auto const metaLayout = MetaStructUtils::GenerateFrom(layout);
		Asserts::CheckBool(metaLayout, "Meta layout is not valid");

		return Layout(metaLayout);
	}
	else
	{
		return Layout(MetaStructPointerType{});
	}
}

aiva::utils::DictBuffer& aiva::utils::DictBuffer::Layout(LayoutStructPointerType const& layout)
{
	if (layout)
	{
		auto const metaLayout = MetaStructUtils::GenerateFrom(layout);
		Asserts::CheckBool(metaLayout, "Meta layout is not valid");

		return Layout(metaLayout);
	}
	else
	{
		return Layout(MetaStructPointerType{});
	}
}

aiva::utils::DictBuffer& aiva::utils::DictBuffer::Layout(MetaStructPointerType const& layout)
{
	if (mLayout)
	{
		mLayout->OnCacheDataChanged().disconnect(boost::bind(&DictBuffer::Layout_OnChanged, this));
	}

	mLayout = layout;

	if (mLayout)
	{
		mLayout->OnCacheDataChanged().connect(boost::bind(&DictBuffer::Layout_OnChanged, this));
	}

	BroadcastCacheDataChanged();
	return *this;
}

void aiva::utils::DictBuffer::Layout_OnChanged()
{
	Asserts::CheckBool(false, "Layout has been changed, please don't do that");
}

aiva::utils::DictBuffer& aiva::utils::DictBuffer::Add(DictStructPointerType const& dictStruct)
{
	Asserts::CheckBool(mLayout, "Layout is not valid");
	Asserts::CheckBool(dictStruct, "Dict struct is not valid");
	Asserts::CheckBool(DictStructUtils::IsMatchingLayout(dictStruct, mLayout), "Dict struct doesn't match the layout");

	dictStruct->OnCacheDataChanged().connect(boost::bind(&DictBuffer::Struct_OnChanged, this));
	mStructs.emplace_back(dictStruct);

	BroadcastCacheDataChanged();
	return *this;
}

aiva::utils::DictBuffer::DictStructPointerType const& aiva::utils::DictBuffer::Get(std::size_t index) const
{
	return mStructs.at(index);
}

std::size_t aiva::utils::DictBuffer::Num() const
{
	return std::size(mStructs);
}

void aiva::utils::DictBuffer::Struct_OnChanged()
{
	Asserts::CheckBool("Struct has been changed, please don't do that");
}
