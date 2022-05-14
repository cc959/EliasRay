#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace std;
using namespace glm;
class Resources
{
public:
	static GLuint LoadShaders(const string vertexPath, const string fragmentPath);
	static GLuint LoadImage(const string imagePath);
	static void computeTangentBasis(std::vector<vec3>& vertices, std::vector<vec2>& uvs, std::vector<vec3>& normals, std::vector<vec3>& tangents, std::vector<vec3>& bitangents);
	static void LoadOBJ(const string path, vector<vec3>& vertices, vector<vec2>& uvs, vector<vec3>& normals);
};
