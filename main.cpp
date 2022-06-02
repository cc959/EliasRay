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

vector<Object> spheres;

void setup(Camera &camera, vector<Object> &objects, int frame)
{

    float time = frame / float(30);

    camera.Position = vec3(sin(time * 20.f) * 7.5, 7.5, cos(time * 20.f) * 7.5);

    float height = 12.5;

    //  camera.Position = vec3(10, 15, 10);
    camera.Rotation = quatLookAt(normalize(camera.Position - vec3(0, 0, 0)), vec3(0, 1, 0));
    camera.fov = 80;

    for (Object sphere : spheres)
        objects.push_back(sphere);

    Object plane;
    plane.data = 2u;
    plane.position = vec4(0, 0, 0, 0);
    plane.size = vec4(0, 1, 0, 0); // normal
    plane.color = vec4(1, 1, 1, 0);
    plane.smoothness = 0; // 0.95 + sin(time * 2) * 0.05;

    objects.push_back(plane);
}

void generateSpheres(int n, float placementRadius)
{

    while (spheres.size() < n)
    {
        float angle = rand() / float(RAND_MAX) * 2 * pi<float>();
        float dist = rand() / float(RAND_MAX) * placementRadius;
        float radius = rand() / float(RAND_MAX) * 3.f;

        vec3 position(cos(angle) * dist, radius, sin(angle) * dist);

        Object sphere;

        for (Object other : spheres)
            if (length(position - vec3(other.position)) < radius + other.size.x)
                goto reject;

        sphere.data = 0u;
        sphere.position = vec4(position, 0);
        sphere.size = vec4(radius, 0, 0, 0);
        sphere.color = vec4(rgbColor(vec3(rand() / float(RAND_MAX) * 360.f, 1, 1)), 0);
        sphere.smoothness = 0;

        spheres.push_back(sphere);

    reject:
        continue;
    }
}

int main()
{

    system("mkdir ./Render");

    system("rm ./Render/*.jpg");

    srand(0);
    generateSpheres(20, 5);

    Renderer r(1920, 1080, setup);
    r.Render(0, 10, 1000);

    system("ffmpeg -y -framerate 30 -i ./Render/image%1d.jpg ./output.mp4");
}
