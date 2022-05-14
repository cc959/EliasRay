#pragma once

#include <glm/glm.hpp>

#include "Transform.h"

using namespace glm;

class Scene;

class Light : public Transform
{
public:
    vec3 Color;
    float LightStrength;
    Light(Transform transform = Transform(), vec3 Color = vec3(1, 1, 1), float LightStrength = 1);
};
