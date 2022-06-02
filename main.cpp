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

    camera.Position = vec3(sin(time * 20.f) * 3, 1, cos(time * 20.f) * 3);

    float height = 12.5;

    //  camera.Position = vec3(10, 15, 10);
    camera.Rotation = quatLookAt(normalize(camera.Position - vec3(0, 0, 0)), vec3(0, 1, 0));
    camera.fov = 80;

    Object ball;
    ball.data = 0u;
    ball.position = vec4(0, 1, 0, 0);
    ball.size = vec4(1, 0, 0, 0);
    ball.color = vec4(0.5, 0.5, 0.5, 0);
    ball.smoothness = 0.8;

    objects.push_back(ball);

    ball.color = vec4(1, 0, 0, 0);
    ball.size /= 2;
    ball.position += vec4(1, -0.5, 1, 0);
    ball.smoothness = 1;

    objects.push_back(ball);

    Object plane;
    plane.data = 2u;
    plane.position = vec4(0, 0, 0, 0);
    plane.size = vec4(0, 1, 0, 0); // normal
    plane.color = vec4(1, 1, 1, 0);
    plane.smoothness = 0; // 0.95 + sin(time * 2) * 0.05;

    objects.push_back(plane);
}

int main()
{

    system("mkdir ./Render");

    system("rm ./Render/*.jpg");

    Renderer r(1920, 1080, setup);
    r.Render(0, 10, 1000);

    system("ffmpeg -y -framerate 30 -i ./Render/image%1d.jpg ./output.mp4");
}
