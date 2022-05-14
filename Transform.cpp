#include "Transform.h"

Transform::Transform(vec3 Position, quat Rotation, vec3 Scale) : Position(Position), Rotation(Rotation), Scale(Scale) {}

mat4 Transform::ModelMatrix()
{
	mat4 scaleMat = scale(Scale);

	mat4 rotationMat = toMat4(Rotation);

	mat4 positionMat = translate(Position);
	return positionMat * rotationMat * scaleMat;
}