#pragma once

#include "pch.h"

namespace aiva
{
	class Renderer final : private boost::noncopyable
	{
	public:
		Renderer();

	private:
		void EnableDebugLayer();
	};
}
