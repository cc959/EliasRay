#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

class Transform
{
public:

	vec3 Position;
	vec3 Scale;
	quat Rotation;

	Transform(vec3 Position = vec3(0.f, 0.f, 0.f), quat Rotation = quat(1, 0, 0, 0), vec3 Scale = vec3(1.f, 1.f, 1.f));

	mat4 ModelMatrix();
};
