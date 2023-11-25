#pragma once
#include <boost/noncopyable.hpp>
#include <aiva/implements.hpp>

namespace aiva
{
	struct object_t : public implements_t<object_t>, private boost::noncopyable
	{

	};
}
