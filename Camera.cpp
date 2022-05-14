#include "Camera.h"

#include <iostream>

Camera::Camera(Transform transform, float aspectRatio, float fov, float nearPlane, float farPlane) : aspectRatio(aspectRatio), fov(fov), nearPlane(nearPlane), farPlane(farPlane)
{
    Position = transform.Position;
	Rotation = transform.Rotation;
	Scale = transform.Scale;
}

mat4 Camera::ProjectionMatrix()
{
    return perspective(radians(fov), aspectRatio, nearPlane, farPlane);
}

mat4 Camera::ViewMatrix()
{
    vec3 forward = mat3(ModelMatrix()) * vec3(0.f, 0.f, 1.f);

    // cout << "position: " << forward.x << " " << forward.y << " " << forward.z << "\n";

    mat4 ViewMatrix = lookAt(
        Position,           // Camera is here
        forward + Position, // and looks here
        vec3(0, 1, 0)       // Head is up (set to 0,-1,0 to look upside-down)
    );
    return ViewMatrix;
}