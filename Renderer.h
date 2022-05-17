#include "Camera.h"
#include "Object.h"
#include "Shader.h"
#include "Texture.h"
#include <GL/glew.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <bits/stdc++.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/matrix.hpp>

using namespace std;
// using namespace sf;
using namespace std::complex_literals;

class Renderer
{

private:
    sf::Font fpsFont;

    sf::RenderTexture output;
    sf::RenderWindow viewport;

    int width, height;
    function<void(Camera &, vector<Object> &, int)> scene;

public:
    Renderer(int width, int height, function<void(Camera &, vector<Object> &, int)> scene);

    void Render(int startFrame, int endFrame, int samples);
};
