#include "Light.h"

Light::Light(Transform transform, vec3 Color, float LightStrength) : Color(Color), LightStrength(LightStrength)
{
    Position = transform.Position;
    Rotation = transform.Rotation;
    Scale = transform.Scale;
}