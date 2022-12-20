#pragma once

#ifndef NOMINMAX
	#define NOMINMAX
#endif

#include <algorithm>

#include <any>

#include <chrono>

#include <cstddef>

#include <cstring>

#include <deque>

#include <exception>

#include <filesystem>

#include <fstream>

#include <functional>

#include <iostream>

#include <iterator>

#include <limits>

#include <list>

#include <map>

#include <memory>

#include <optional>

#include <ratio>

#include <regex>

#include <set>

#include <stdexcept>

#include <string>

#include <type_traits>

#include <typeindex>

#include <unordered_map>

#include <unordered_set>

#include <utility>

#include <vector>

#include <boost/bind.hpp>

#include <boost/function.hpp>

#include <boost/container_hash/hash.hpp>

#include <boost/core/noncopyable.hpp>

#include <boost/core/span.hpp>

#include <boost/signals2.hpp>

#include <Unknwn.h>

#include <winrt/base.h>

#include <winrt/Windows.ApplicationModel.Core.h>

#include <winrt/Windows.System.Diagnostics.h>

#include <winrt/Windows.UI.Core.h>

#include <d3d12.h>

#include <dxgi1_6.h>

#include <d3dcompiler.h>

#define GLM_FORCE_RIGHT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#define GLM_FORCE_SIZE_T_LENGTH
#define GLM_FORCE_XYZW_ONLY
#include <glm/glm.hpp>

#include <glm/ext.hpp>

#include <glm/gtx/std_based_type.hpp>

#include <nlohmann/json.hpp>
#define TINYGLTF_NO_INCLUDE_JSON

#include <tinyutf8/tinyutf8.h>

#include <stb/stb_image.h>
#define TINYGLTF_NO_INCLUDE_STB_IMAGE

#include <stb/stb_image_write.h>
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE

#include <tinygltf/tiny_gltf.h>

using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::UI::Core;
