#include <pch.h>
#include <aiva/utils/math_utils.h>

namespace aiva::utils
{
	glm::mat4 const& MathUtils::BlenderToDirectxMatrix()
	{
		static const auto matrix = glm::mat4
		{
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, -1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f },
		};

		return matrix;
	}
}
