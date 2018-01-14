#pragma once

#include <math.h>

#define _USE_MATH_DEFINES
#include <cmath>

#ifdef BUILD_WIN_GLES
	#include <corecrt_math_defines.h>
#elif BUILD_WIN_GLFW
	#include <corecrt_math_defines.h>
#elif BUILD_OSX

#endif


// GLM
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
