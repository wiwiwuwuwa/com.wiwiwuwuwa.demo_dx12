#pragma once
#include <pch.h>

namespace aiva::utils
{
	struct ObjectFactory;
}

namespace aiva::utils
{
	struct AObject : private boost::noncopyable, public std::enable_shared_from_this<AObject>
	{
	// ----------------------------------------------------
	// Main

	public:
		using FactoryType = ObjectFactory;

	protected:
		AObject();

	public:
		virtual ~AObject();
	};
}
