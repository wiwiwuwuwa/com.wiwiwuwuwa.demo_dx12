#include <pch.h>
#include <aiva/utils/layout_buffer.h>

#include <aiva/utils/layout_struct.h>

aiva::utils::LayoutBuffer::LayoutBuffer() : AObject{}, IObjectChangeable{}
{

}

aiva::utils::LayoutBuffer::~LayoutBuffer()
{

}

aiva::utils::LayoutBuffer::LayoutStructPointerType const& aiva::utils::LayoutBuffer::Layout() const
{
	return mLayout;
}

aiva::utils::LayoutBuffer& aiva::utils::LayoutBuffer::Layout(LayoutStructPointerType const& layout)
{
	if (mLayout)
	{
		mLayout->OnChanged().disconnect(boost::bind(&LayoutBuffer::Layout_OnChanged, this));
	}

	mLayout = layout;

	if (mLayout)
	{
		mLayout->OnChanged().connect(boost::bind(&LayoutBuffer::Layout_OnChanged, this));
	}

	OnChanged()();
	return *this;
}

void aiva::utils::LayoutBuffer::Layout_OnChanged()
{
	OnChanged()();
}

std::size_t aiva::utils::LayoutBuffer::Stride() const
{
	return mStride;
}

aiva::utils::LayoutBuffer& aiva::utils::LayoutBuffer::Stride(std::size_t const stride)
{
	if (mStride != stride)
	{
		mStride = stride;
		OnChanged()();
	}

	return *this;
}

std::size_t aiva::utils::LayoutBuffer::Offset() const
{
	return mOffset;
}

aiva::utils::LayoutBuffer& aiva::utils::LayoutBuffer::Offset(std::size_t const offset)
{
	if (mOffset != offset)
	{
		mOffset = offset;
		OnChanged()();
	}

	return *this;
}

std::size_t aiva::utils::LayoutBuffer::Size() const
{
	return mSize;
}

aiva::utils::LayoutBuffer& aiva::utils::LayoutBuffer::Size(std::size_t const size)
{
	if (mSize != size)
	{
		mSize = size;
		OnChanged()();
	}

	return *this;
}
