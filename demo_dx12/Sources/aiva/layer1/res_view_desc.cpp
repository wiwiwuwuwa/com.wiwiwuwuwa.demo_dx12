#include <pch.h>
#include <aiva/layer1/res_view_desc.h>

namespace aiva::layer1
{
	ResViewDesc::ResViewDesc()
	{

	}

	ResViewDesc::ResViewDesc(ResourceViewHeapTypeShared const& heap, std::string const& name) : Heap{ heap }, Name{ name }
	{

	}
}
