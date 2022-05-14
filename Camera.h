#pragma once
#include "Transform.h"

#include <glm/glm.hpp>

using namespace glm;
using namespace std;

class Scene;

class Camera : public Transform
{
public:
	float aspectRatio;
	float fov;
	float nearPlane;
	float farPlane;

	Camera(Transform transform = Transform(), float aspectRatio = 16.f / 9.f, float fov = 45.f, float nearPlane = 0.1f, float farPlane = 1000.f);

	mat4 ProjectionMatrix();

	mat4 ViewMatrix();
};
