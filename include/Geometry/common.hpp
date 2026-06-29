#ifndef GEOMETRY_COMMON_HPP
#define GEOMETRY_COMMON_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace Celer {
	namespace Geometry {

		glm::mat4 projection(GLFWwindow*& window, float fovDegrees, float n, float f);

	}
}


#endif