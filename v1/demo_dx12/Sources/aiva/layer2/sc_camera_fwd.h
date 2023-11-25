#pragma once
#include <pch.h>

namespace aiva::layer2
{
	struct ScCamera;

	using ScCameraType = ScCamera;
	using ScCameraTypeShared = std::shared_ptr<ScCameraType>;
	using ScCameraTypeWeak = std::weak_ptr<ScCameraType>;
}
