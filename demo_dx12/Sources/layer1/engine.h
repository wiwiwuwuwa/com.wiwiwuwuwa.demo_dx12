#pragma once
#include <pch.h>

#include <memory>
#include <boost/core/noncopyable.hpp>
#include <winrt/base.h>

namespace aiva::layer0
{
	struct App;
}

namespace aiva::layer1
{
	struct Renderer;
}

namespace aiva::layer1
{
	struct Engine final : private boost::noncopyable
	{
	// ----------------------------------------------------
	// Engine

	public:
		Engine();
		~Engine();

		winrt::com_ptr<aiva::layer0::App> const& App() const;

	private:
		void OnStart();

		void OnUpdate();

		void OnFinish();

		winrt::com_ptr<aiva::layer0::App> mApp{};

		std::unique_ptr<aiva::layer1::Renderer> mRenderer{};
	};
}
