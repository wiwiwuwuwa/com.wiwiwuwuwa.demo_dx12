#pragma once
#include <aiva2/_core.hpp>

namespace aiva2
{
	struct object_t : private boost::noncopyable
	{
		// ------------------------------------------------

	protected:
		virtual ~object_t() = default;

		// ------------------------------------------------
	};
}
