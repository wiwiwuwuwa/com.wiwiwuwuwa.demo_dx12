#pragma once
#include <pch.h>

#include <aiva2/core/object_fwd.h>

namespace aiva2::core
{
	struct object_t : private boost::noncopyable
	{
		// ------------------------------------------------

	public:
		virtual ~object_t() = default;

		// ------------------------------------------------
	};
}