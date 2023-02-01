#pragma once
#include <aiva2/base.hpp>

namespace aiva2::core
{
	struct object_t : private boost::noncopyable
	{
		// ------------------------------------------------

	protected:
		virtual ~object_t() = default;

		// ------------------------------------------------
	};
}
