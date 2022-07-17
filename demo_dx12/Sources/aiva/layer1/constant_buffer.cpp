#include <pch.h>
#include <aiva/layer1/constant_buffer.h>

#include <aiva/layer1/constant_packer.h>

aiva::layer1::ConstantBuffer::ConstantBuffer(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeLowLevelData();
}

aiva::layer1::ConstantBuffer::~ConstantBuffer()
{
	TerminateLowLevelData();
}

aiva::layer1::ConstantBuffer& aiva::layer1::ConstantBuffer::MarkAsChanged(aiva::layer1::ConstantBuffer::EDirtyFlags const dirtyFlags /*=EDirtyFlags::All*/)
{
	mChangesDetector.MarkAsChanged(dirtyFlags);
	return *this;
}

void aiva::layer1::ConstantBuffer::InitializeLowLevelData()
{
	mChangesDetector.OnFlushChanges().connect(boost::bind(&aiva::layer1::ConstantBuffer::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::ConstantBuffer::TerminateLowLevelData()
{
	mChangesDetector.OnFlushChanges().disconnect(boost::bind(&aiva::layer1::ConstantBuffer::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::ConstantBuffer::RefreshLowLevelData(EDirtyFlags const dirtyFlags)
{
	auto x = aiva::layer1::ConstantPacker::PackConstants(mValues.cbegin(), mValues.cend());
	auto y = x;
}
