#include <pch.h>
#include <aiva/utils/meta_buffer.h>

#include <aiva/utils/meta_struct.h>

aiva::utils::MetaBuffer::MetaBuffer() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::MetaBuffer::~MetaBuffer()
{

}

aiva::utils::MetaBuffer::MetaStructPointerType const& aiva::utils::MetaBuffer::Layout() const
{
	return mLayout;
}

aiva::utils::MetaBuffer& aiva::utils::MetaBuffer::Layout(MetaStructPointerType const& layout)
{
	if (mLayout)
	{
		mLayout->OnCacheDataChanged().disconnect(boost::bind(&MetaBuffer::Layout_OnChanged, this));
	}

	mLayout = layout;

	if (mLayout)
	{
		mLayout->OnCacheDataChanged().connect(boost::bind(&MetaBuffer::Layout_OnChanged, this));
	}

	BroadcastCacheDataChanged();
	return *this;
}

void aiva::utils::MetaBuffer::Layout_OnChanged()
{
	BroadcastCacheDataChanged();
}

std::size_t aiva::utils::MetaBuffer::Count() const
{
	return mCount;
}

aiva::utils::MetaBuffer& aiva::utils::MetaBuffer::Count(std::size_t const count)
{
	if (mCount != count)
	{
		mCount = count;
		BroadcastCacheDataChanged();
	}

	return *this;
}
