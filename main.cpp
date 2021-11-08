#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <GL/glew.h>
#include <bits/stdc++.h>
#include <glm/gtx/transform.hpp>
#include "shader.h"

using namespace std;
//using namespace sf;
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

int main()
{

	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Shader", sf::Style::Default, sf::ContextSettings{24, 8, 0, 4, 6});
	//window.setVerticalSyncEnabled(true);

	window.setActive(true);

	sf::Clock elapsedClock, frameClock;

	sf::Font fpsFont;
	if (!fpsFont.loadFromFile("res/Fonts/Roboto-Medium.ttf"))
		cout << "Error loading font\n";

	glewInit();

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	Shader def("res/vert.glsl", "res/frag.glsl");

	bool running = true;
	while (running)
	{
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
			}
		}

		float deltaTime = frameClock.getElapsedTime().asSeconds();
		frameClock.restart();

		float fps = 1.f / deltaTime;

		def.reload();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(def);

		glUniform1f(glGetUniformLocation(def, "u_time"), elapsedClock.getElapsedTime().asSeconds());
		auto mousePos = sf::Mouse::getPosition(window);
		glUniform2f(glGetUniformLocation(def, "u_mouse"), mousePos.x, window.getSize().y - mousePos.y);
		glUniform2f(glGetUniformLocation(def, "u_resolution"), window.getSize().x, window.getSize().y);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,		  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,		  // size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0,		  // stride
			(void *)0 // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);

		window.pushGLStates();

		sf::Text text;

		// select the font
		text.setFont(fpsFont); // font is a sf::Font

		// set the string to display
		//text.setString("Fps: " + std::to_string(fps));
		string name((char *)glGetString(GL_VENDOR));

		text.setString(name + " " + std::to_string(window.getSettings().antialiasingLevel) + "\nFps: " + std::to_string(fps));
		// set the character size
		text.setCharacterSize(24); // in pixels, not points!

		// set the color
		text.setFillColor(sf::Color::White);

		// set the text style
		text.setStyle(sf::Text::Bold);

		// inside the main loop, between window.clear() and window.display()
		window.draw(text);

		window.popGLStates();

		window.display();
	}

	return 0;
}
