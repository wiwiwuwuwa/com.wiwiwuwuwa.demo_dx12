#include <pch.h>
#include <aiva/utils/i_object_cacheable.h>

namespace aiva::utils
{
	IObjectCacheable::IObjectCacheable(bool const initAsChanged) : TObjectCacheable<>{ initAsChanged }
	{

	}

	IObjectCacheable::~IObjectCacheable()
	{

	}
}
