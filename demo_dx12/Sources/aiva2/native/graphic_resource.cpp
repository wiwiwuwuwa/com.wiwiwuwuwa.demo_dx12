#include <pch.h>
#include <aiva2/native/graphic_resource.hpp>

namespace aiva2::native
{
	graphic_resource_t::graphic_resource_t(engine_t& engine) : impl_type{ engine }
	{
		
	}

	graphic_resource_t::graphic_resource_t(engine_t& engine, winrt::com_ptr<ID3D12Resource> const& resource) : impl_type{ engine }, m_resource{ resource }
	{
		
	}

	graphic_resource_t::~graphic_resource_t()
	{
		
	}
}
