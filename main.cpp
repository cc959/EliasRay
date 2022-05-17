#include "Renderer.h"
#include <bits/stdc++.h>
#include <glm/gtx/color_space.hpp>
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

    float time = frame / float(30);

    camera.Position = vec3(sin(time * 0.5f) * 30, 12.5 + sin(time * 0.5f) * 2.5, cos(time * 0.5f) * 30);

    float height = 12.5;

    //  camera.Position = vec3(10, 15, 10);
    camera.Rotation = quatLookAt(normalize(camera.Position - vec3(0, 5, 0)), vec3(0, 1, 0));
    camera.fov = 80;

    for (float x = -5; x <= 5; x++)
        for (float y = -5; y <= 5; y++)
        {

            float h = height + sin(x + 100 * y) * 5;

            float size = 0.85f + 0.25f * cos(x * 453 - y * 88);

            Object balls;
            balls.position = vec4(4.f * x, size + h - pow(mod(time * 6 + (sin(x) * 100.f + sin(y) * 50.f) * 4 + sqrt(h), 2.f * sqrt(h)) - sqrt(h), 2.f), 4.f * y, 0);
            balls.color = vec4(rgbColor(vec3(mod(x * 9538.f + y * 234834.f, 360.f), 1, 1)) * 5.f, usin(x * 4565.f + y * 6456.f) * 0.55);
            balls.size = vec4(size / 2, size, size / 2.f, 0);
            balls.smoothness = 0.95 + cos(x * 32 + y * 65) * 0.05;
            balls.setType(int(sin(2 * x + 5 * y) + 1));

            objects.push_back(balls);
        }

    Object plane;
    plane.data = 1u;
    // plane.setType(1);)
    plane.position = vec4(0, 0, 0, 0);
    plane.size = vec4(50, 0.01, 50, 0);
    plane.color = vec4(0, 0, 0, 0);
    plane.smoothness = 0.95 + sin(time * 2) * 0.05;

    objects.push_back(plane);
}

int main()
{

    system("mkdir ./Render");

    // system("rm ./Render/*.jpg");

    Renderer r(1280, 720, setup);
    r.Render(0, 300, 30);

    system("ffmpeg -y -framerate 30 -i ./Render/image%1d.jpg ./output.mp4");
}
