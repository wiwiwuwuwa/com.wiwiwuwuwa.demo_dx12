#pragma once
#include <pch.h>

#include <aiva/layer1/res_view_desc_fwd.h>
#include <aiva/layer1/resource_view_heap_fwd.h>

namespace aiva::layer1
{
	struct ResViewDesc final
	{
	public:
		ResViewDesc();

		ResViewDesc(ResourceViewHeapTypeShared const& heap, std::string const& name);

	public:
		ResourceViewHeapTypeShared Heap{};

		std::string Name{};
	};
}
