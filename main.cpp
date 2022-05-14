#include "Camera.h"
#include "Player.h"
#include "Shader.h"
#include "Texture.h"
#include <GL/glew.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <bits/stdc++.h>
#include <glm/gtx/transform.hpp>
#include <glm/matrix.hpp>

using namespace std;
// using namespace sf;
using namespace std::complex_literals;

// An quad covering the screen
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,
    -1.0f,
    0.0f,

    1.0f,
    -1.0f,
    0.0f,

    -1.0f,
    1.0f,
    0.0f,

    1.0f,
    -1.0f,
    0.0f,

    1.0f,
    1.0f,
    0.0f,

    -1.0f,
    1.0f,
    0.0f,
};
const double pi = 3.14159265359;

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

struct alignas(16) Object
{
    vec4 position;
    vec4 size;
    vec4 color;
    float smoothness;
    float metallic;
    uint data;
};

int main()
{

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "EliasRay", sf::Style::Default, sf::ContextSettings{24, 8, 0, 4, 6});
    window.setVerticalSyncEnabled(true);

    window.setActive(true);

    sf::Clock elapsedClock, frameClock;

    sf::Font fpsFont;
    if (!fpsFont.loadFromFile("res/Fonts/Roboto-Medium.ttf"))
        cout << "Error loading font\n";

    glewInit();
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Player player(&window);
    Camera camera = Camera();

    Texture skybox("res/SkyBox.jpg");
    skybox.SetFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    Shader def("res/vert.glsl", "res/frag.glsl");

    Object objects[3] = {{vec4(1, 1, 1, 0), vec4(1, 0, 0, 0), vec4(1, 0, 0, 1), 0, 1, 0u}, {vec4(1, 10, 1, 0), vec4(4, 1, 4, 0), vec4(1, 1, 1, 1), 1, 1, 1u}, {vec4(0, 0, 0, 0), vec4(20, 0.1, 20, 0), vec4(0, 0.5, 0, 1), 0.5, 0.1, 1u}};

    vec3 lightDir(0.5, -1, 0.2);
    lightDir = normalize(lightDir);

    GLuint objectBuffer;
    glGenBuffers(1, &objectBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, objectBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Object) * 3, &objects[0], GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, objectBuffer);

    int frames = 0;
    bool renderMode = 0;

    bool running = true;
    while (running)
    {
        frames++;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                glClear(GL_COLOR_BUFFER_BIT);
                camera.aspectRatio = event.size.width / event.size.height;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::C)
                {
                    {
                        glClear(GL_COLOR_BUFFER_BIT);
                        renderMode = !renderMode;
                    }
                }
                if (event.key.code == sf::Keyboard::S && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                    int width = window.getSize().x;
                    int height = window.getSize().y;

                    vector<uint8> pixels(4 * width * height);
                    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

                    sf::Image output;
                    output.create(width, height, &pixels[0]);
                    output.flipVertically();
                    output.saveToFile("image.jpg");
                }
            }
        }

        if (!renderMode)
            frames = 1;

        float deltaTime = frameClock.getElapsedTime().asSeconds();
        frameClock.restart();

        float fps = 1.f / deltaTime;

        if (!renderMode)
            player.UpdateTransform(deltaTime);

        camera = player.transform;

        mat4 _CameraInverseProjection = inverse(camera.ProjectionMatrix());
        mat4 _CameraToWorld = inverse(camera.ViewMatrix());

        // def.reload();
        glClear(GL_DEPTH_BUFFER_BIT);

        def.BindShader();

        glUniform1f(glGetUniformLocation(def, "u_time"), elapsedClock.getElapsedTime().asSeconds());
        auto mousePos = sf::Mouse::getPosition(window);
        glUniform2f(glGetUniformLocation(def, "u_mouse"), mousePos.x, window.getSize().y - mousePos.y);
        glUniform2f(glGetUniformLocation(def, "u_resolution"), window.getSize().x, window.getSize().y);
        glUniformMatrix4fv(glGetUniformLocation(def, "_CameraInverseProjection"), 1, false, &_CameraInverseProjection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(def, "_CameraToWorld"), 1, false, &_CameraToWorld[0][0]);
        glUniform1i(glGetUniformLocation(def, "n"), 3);
        glUniform1i(glGetUniformLocation(def, "frames"), frames);
        glUniform3f(glGetUniformLocation(def, "lightDir"), lightDir[0], lightDir[1], lightDir[2]);

        glActiveTexture(GL_TEXTURE1);
        skybox.BindTexture();
        glUniform1i(glGetUniformLocation(def, "skybox"), 1);
        glActiveTexture(GL_TEXTURE0);

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,        // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);

        window.pushGLStates();

        if (!renderMode)
        {

            sf::Text text;

            // select the font
            text.setFont(fpsFont); // font is a sf::Font

            // set the string to display
            // text.setString("Fps: " + std::to_string(fps));
            string name((char *)glGetString(GL_VENDOR));

            text.setString(name + " " + std::to_string(window.getSettings().antialiasingLevel) + "\nFps: " + std::to_string(fps));
            // set the character size
            text.setCharacterSize(24); // in pixels, not points!

            // set the color
            text.setFillColor(sf::Color::Black);

            // set the text style
            text.setStyle(sf::Text::Bold);

            // inside the main loop, between window.clear() and window.display()
            window.draw(text);
        }

        window.popGLStates();

        window.display();
    }

    return 0;
}
