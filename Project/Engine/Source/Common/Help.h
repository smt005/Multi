#pragma once

#include "Common/IncludesMatem.h"

namespace help
{
using namespace glm;

bool intersection(vec3 start1, vec3 end1, vec3 start2, vec3 end2, vec3 *out_intersection);
float random_f(const float& min  = 0.0f, const float& max = 1.0f, const float& accuracy = 1000.0f);
int random_i(const int& min = 0, const int& max = 1.0);
}
