#pragma once

#ifndef NOMINMAX
	#define NOMINMAX
#endif

#include <memory>
#include <boost/core/noncopyable.hpp>
#include <Unknwn.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <tinyutf8/tinyutf8.h>

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::UI::Core;
