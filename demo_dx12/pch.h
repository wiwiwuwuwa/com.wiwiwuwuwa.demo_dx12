#pragma once

#ifndef NOMINMAX
	#define NOMINMAX
#endif

#include <algorithm>
#include <any>
#include <cstddef>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/core/span.hpp>
#include <boost/signals2.hpp>
#include <Unknwn.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.System.Diagnostics.h>
#include <winrt/Windows.UI.Core.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
#include <tinyutf8/tinyutf8.h>

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::UI::Core;
