#pragma once

#ifndef NOMINMAX
	#define NOMINMAX
#endif

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include <boost/core/noncopyable.hpp>
#include <Unknwn.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.System.Diagnostics.h>
#include <winrt/Windows.UI.Core.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <tinyutf8/tinyutf8.h>
#include <nlohmann/json.hpp>

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::UI::Core;
