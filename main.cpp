#include "Renderer.h"
#include <bits/stdc++.h>

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

    camera.Position = vec3(sin(time) * 10, usin(time / 2) * 20, cos(time) * 10);
    camera.Rotation = quatLookAt(normalize(camera.Position), vec3(0, 1, 0));
    camera.fov = 80;

    Object balls;
    // balls.setType(0);
    balls.position = vec4(0, 1, 0, 0);
    balls.color = vec4(1, 0, 0, 0);
    balls.size = vec4(sin(time * 1.5) + 1, 0, 0, 0);
    balls.smoothness = 1;

    Object plane;
    plane.data = 2u;
    // plane.setType(1);
    plane.position = vec4(0, 0, 0, 0);
    plane.size = vec4(8, 0.1, 20, 0);
    plane.color = vec4(0, 0.5, 1, 0);
    plane.smoothness = 0.9;

    objects.push_back(plane);

    // objects.push_back(balls);
}

int main()
{

    system("mkdir ./Render");

    system("rm ./Render/*.jpg");

    Renderer r(2000, 2000, setup);

    r.Render(0, 1, 30);

    // system("ffmpeg -y -framerate 24 -i ./Render/image%1d.jpg ./output.mp4");
}
