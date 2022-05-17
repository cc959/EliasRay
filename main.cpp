#include "Renderer.h"
#include <bits/stdc++.h>
#include <glm/gtx/euler_angles.hpp>

// vector<Object> balls = {, {vec4(0, 5, 0.5, 0), vec4(6, 5, 0.1, 0), vec4(1, 1, 1, 1), 0.995, 1u},, {vec4(6, 5, 0.5, 0), vec4(0.2, 5, 0.2, 0), vec4(0, 0, 0, 1), 0.9, 1u}, {vec4(-6, 5, 0.5, 0), vec4(0.2, 5, 0.2, 0), vec4(0, 0, 0, 1), 0.9, 1u}, {vec4(0, 10, 0.5, 0), vec4(6, 0.2, 0.2, 0), vec4(0, 0, 0, 1), 0.9, 1u}};

float usin(float x)
{
    return (sin(x) + 1.f) / 2.f;
}
float ucos(float x)
{
    return (cos(x) + 1.f) / 2.f;
}

void setup(Camera &camera, vector<Object> &objects, int frame)
{

    float time = frame / float(24);

    camera.Position = vec3(sin(time) * 25, 12.5 + sin(time) * 2.5, cos(time) * 25);

    float h = 10;

    //  camera.Position = vec3(10, 15, 10);
    camera.Rotation = quatLookAt(normalize(camera.Position - vec3(0, 5, 0)), vec3(0, 1, 0));
    camera.fov = 80;

    for (float x = -5; x <= 5; x++)
        for (float y = -5; y <= 5; y++)
        {
            Object balls;
            balls.position = vec4(4.f * x, h - pow(mod(float(frame) / 3.f + (sin(x) * 100.f + sin(y) * 50.f) * 4 + sqrt(h), 2.f * sqrt(h)) - sqrt(h), 2.f), 4.f * y, 0);
            balls.color = vec4(1, 0, 0, 0);
            balls.size = vec4(1, 0, 0, 0);
            balls.smoothness = 0.99;

            objects.push_back(balls);
        }

    Object plane;
    plane.data = 1u;
    // plane.setType(1);)
    plane.position = vec4(0, 0, 0, 0);
    plane.size = vec4(50, 0.1, 50, 0);
    plane.color = vec4(0, 0.5, 0.9, 0);
    plane.smoothness = 0.95;

    objects.push_back(plane);
}

int main()
{

    system("mkdir ./Render");

    system("rm ./Render/*.jpg");

    Renderer r(1280, 720, setup);
    r.Render(0, 300, 20);

    system("ffmpeg -y -framerate 30 -i ./Render/image%1d.jpg ./output.mp4");
}
