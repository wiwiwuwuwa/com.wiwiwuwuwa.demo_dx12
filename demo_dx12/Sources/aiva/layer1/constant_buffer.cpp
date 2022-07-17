#include <pch.h>
#include <aiva/layer1/constant_buffer.h>

aiva::layer1::ConstantBuffer::ConstantBuffer(aiva::layer1::Engine const& engine) : mEngine{ engine }
{
	InitializeLowLevelData();
}

aiva::layer1::ConstantBuffer::~ConstantBuffer()
{
	TerminateLowLevelData();
}

aiva::layer1::ConstantBuffer& aiva::layer1::ConstantBuffer::IncrementChanges(aiva::layer1::ConstantBuffer::EDirtyFlags const& dirtyFlags)
{
	mChangesCounter.IncrementChanges(dirtyFlags);
	return *this;
}

aiva::layer1::ConstantBuffer& aiva::layer1::ConstantBuffer::DecrementChanges(aiva::layer1::ConstantBuffer::EDirtyFlags const& dirtyFlags)
{
	mChangesCounter.DecrementChanges(dirtyFlags);
	return *this;
}

void aiva::layer1::ConstantBuffer::InitializeLowLevelData()
{
	mChangesCounter.OnChangesFinished().connect(boost::bind(&aiva::layer1::ConstantBuffer::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::ConstantBuffer::TerminateLowLevelData()
{
	mChangesCounter.OnChangesFinished().disconnect(boost::bind(&aiva::layer1::ConstantBuffer::RefreshLowLevelData, this, boost::placeholders::_1));
}

void aiva::layer1::ConstantBuffer::RefreshLowLevelData(EDirtyFlags const dirtyFlags)
{
	auto x = dirtyFlags;
	auto y = x;
	auto z = y;
}
