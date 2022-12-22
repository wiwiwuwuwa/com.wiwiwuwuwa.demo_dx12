#pragma once
#include <pch.h>

#include <aiva2/engine/i_object_fwd.h>

namespace aiva2::engine
{
	struct IObject : private boost::noncopyable
	{
	// ----------------------------------------------------

	public:
		virtual ~IObject() = default;

	// ----------------------------------------------------

	public:
		virtual void Init() {};

		virtual void Shut() {};
	};
}
