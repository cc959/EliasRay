#include "Player.h"

Player::Player(sf::Window *gameWindow, float playerSpeed, float acceleration, float mouseSens)
{
    this->gameWindow = gameWindow;
    this->playerSpeed = playerSpeed;
    this->acceleration = acceleration;
    this->mouseSens = mouseSens;

    verticalAngle = 0;
    horizontalAngle = 0;
    velocity = vec3(0, 0, 0);
    updateMouseLock(true);
}

Transform Player::UpdateTransform(float deltaTime)
{
    if (mouseLocked)
    {
        auto mouse = getMouseMov();

        horizontalAngle += mouseSens * deltaTime * mouse.x;
        verticalAngle += mouseSens * deltaTime * -mouse.y;
        verticalAngle = glm::clamp(verticalAngle, radians(-89.9f), radians(89.9f));
    }

    vec3 direction = normalize(vec3(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)));

    vec3 right = glm::vec3(
        sin(horizontalAngle - 3.141f / 2.0f),
        0,
        cos(horizontalAngle - 3.141f / 2.0f));

    vec3 up = vec3(0, 1, 0);

    vec3 forward = cross(up, right);

    if (mouseLocked)
        transform.Position += updateVelocity(forward, right, up, deltaTime) * deltaTime;

    transform.Rotation = angleAxis(horizontalAngle, vec3(0, 1, 0)) * angleAxis(verticalAngle, vec3(1, 0, 0));

    return transform;
}

vec2 Player::getMouseMov()
{
    auto mousePos = sf::Mouse::getPosition(*gameWindow);
    int offSetX = mousePos.x - gameWindow->getSize().x / 2;
    int offSetY = mousePos.y - gameWindow->getSize().y / 2;

    sf::Mouse::setPosition(sf::Vector2i(gameWindow->getSize().x / 2, gameWindow->getSize().y / 2), *gameWindow);
    return vec2(offSetX, offSetY);
}

void Player::updateMouseLock(bool mouseLocked)
{
    this->mouseLocked = mouseLocked;
    gameWindow->setMouseCursorVisible(!mouseLocked);
    if (mouseLocked)
        sf::Mouse::setPosition(sf::Vector2i(gameWindow->getSize().x / 2, gameWindow->getSize().y / 2), *gameWindow);
}

vec3 Player::updateVelocity(vec3 forward, vec3 right, vec3 up, float deltaTime)
{
    vec3 offset(0, 0, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        offset += forward * playerSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        offset += -forward * playerSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        offset += right * playerSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        offset += -right * playerSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        offset += up * playerSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        offset += -up * playerSpeed;

    velocity += (offset - velocity) * acceleration * 60.f * deltaTime;

    return velocity;
}