#pragma once
#include <pch.h>

#include <aiva/layer1/res_view_desc.h>
#include <aiva/layer2/deffered_buffer_fwd.h>

namespace aiva::layer2
{
	struct DefferedBuffer final
	{
		std::vector<aiva::layer1::ResViewDescType> RTs{};

		std::vector<aiva::layer1::ResViewDescType> DSs{};
	};
}
