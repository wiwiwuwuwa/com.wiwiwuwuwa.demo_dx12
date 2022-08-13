#pragma once
#include <pch.h>

#include <aiva/utils/t_object_cacheable.h>

namespace aiva::utils
{
	struct IObjectCacheable : public TObjectCacheable<>
	{
	// ----------------------------------------------------
	// Main

	protected:
		IObjectCacheable(bool const initAsChanged);

	public:
		~IObjectCacheable() override;
	};
}
