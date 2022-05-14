#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "GlStorage.h"

using namespace glm;
using namespace std;

class Mesh : GlStorage
{
public:
	GLuint positionbuffer = 0;
	GLuint uvbuffer = 0;
	GLuint normalbuffer = 0;
	GLuint tangentbuffer = 0;
	GLuint bitangentbuffer = 0;

	int vertexCount = 0;

	Mesh(vector<vec3> &positions, vector<vec2> &uvs, vector<vec3> &normals);
	Mesh(vector<vec3> &positions, vector<vec2> &uvs, vector<vec3> &normals, vector<vec3> &tangents, vector<vec3> &bitangents);
	Mesh(const string &path);
	Mesh();

	Mesh(Mesh &&other);
	Mesh(const Mesh &other);
	Mesh &operator=(Mesh &&other);
	Mesh &operator=(const Mesh &other);

	bool EnableArrayAttributes();

	bool DisableArrayAttributes();

	~Mesh();

	static Mesh quad;

private:
	bool Release() override;

	template <class U>
	void Copy(U &&other);
};
