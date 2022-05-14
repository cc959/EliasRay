#pragma once
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Transform.h"

using namespace std;
using namespace glm;

class Player
{
public:
    Transform transform;

    Player(sf::Window *gameWindow, float playerSpeed = 40.f, float acceleration = 0.25f, float mouseSens = -0.05f);

    Transform UpdateTransform(float deltaTime);

    void updateMouseLock(bool mouseLocked);

private:
    float playerSpeed;
    float acceleration;
    float mouseSens;

    float verticalAngle;
    float horizontalAngle;

    mat4 projectionMatrix;

    vec3 velocity;
    sf::Window *gameWindow;
    bool mouseLocked;
    vec2 getMouseMov();
    vec3 updateVelocity(vec3 forward, vec3 right, vec3 up, float deltaTime);
};
