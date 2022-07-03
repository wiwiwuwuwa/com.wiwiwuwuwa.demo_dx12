#pragma once
#include <pch.h>
#include <boost/signals2.hpp>

namespace aiva::utils
{
	using EvAction = boost::signals2::signal<void()>;
}
