#include "Resources.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;
using namespace glm;

GLuint Resources::LoadShaders(const string vertexPath, const string fragmentPath) {
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertexPath);

	if (VertexShaderStream.is_open()) {
		stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		cout << "Impossible to open \"" << vertexPath << "\" Are you in the right directory?" << endl;
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragmentPath);

	if (FragmentShaderStream.is_open()) {
		stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}
	else {
		cout << "Impossible to open \"" << fragmentPath << "\" Are you in the right directory?" << endl;
		getchar();
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	cout << "Compiling shader: \"" << vertexPath << "\"\n";
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	cout << "Compiling shader: \"" << fragmentPath << "\"\n";
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint Resources::LoadImage(const string imagePath)
{
	sf::Image image;

	if (image.loadFromFile(imagePath))
		cout << "Loaded \"" << imagePath << "\" successfully\n";

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}

void Resources::computeTangentBasis(std::vector<vec3>& vertices, std::vector<vec2>& uvs, std::vector<vec3>& normals, std::vector<vec3>& tangents, std::vector<vec3>& bitangents)
{
	for (int i = 0; i < vertices.size(); i += 3) {
		// Shortcuts for vertices
		vec3& v0 = vertices[i + 0];
		vec3& v1 = vertices[i + 1];
		vec3& v2 = vertices[i + 2];

		// Shortcuts for UVs
		vec2& uv0 = uvs[i + 0];
		vec2& uv1 = uvs[i + 1];
		vec2& uv2 = uvs[i + 2];

		// Edges of the triangle : position delta
		vec3 deltaPos1 = v1 - v0;
		vec3 deltaPos2 = v2 - v0;

		// UV delta
		vec2 deltaUV1 = uv1 - uv0;
		vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

		// Set the same tangent for all three vertices of the triangle.
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		// Same thing for bitangents
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}
}

void Resources::LoadOBJ(const string path, vector<vec3>& vertices, vector<vec2>& uvs, vector<vec3>& normals)
{
	ifstream file(path);

	vector<int> vertexIndices, uvIndices, normalIndices;
	vector<vec3> temp_vertices;
	vector<vec2> temp_uvs;
	vector<vec3> temp_normals;

	while (!file.eof())
	{
		char line[128];
		file.getline(line, 128);

		stringstream s;
		s << line;

		string prefix;
		s >> prefix;

		if (prefix == "v") {
			float x, y, z;
			s >> x >> y >> z;
			temp_vertices.push_back(vec3(x, y, z));
		}
		else if (prefix == "vt") {
			float x, y;
			s >> x >> y;
			temp_uvs.push_back(vec2(x, 1 - y));
		}
		else if (prefix == "vn") {
			float x, y, z;
			s >> x >> y >> z;
			temp_normals.push_back(vec3(x, y, z));
		}
		else if (prefix == "f")
		{
			vector<string> nums;

			while (!s.eof())
			{
				char c = s.get();
				if (c == ' ' || c == '/')
					nums.push_back("");
				else
					nums.back().append(1, c);
			}

			nums.back().pop_back();

			vertexIndices.push_back(stoi(nums[0]));
			uvIndices.push_back(stoi(nums[1]));
			normalIndices.push_back(stoi(nums[2]));

			vertexIndices.push_back(stoi(nums[3]));
			uvIndices.push_back(stoi(nums[4]));
			normalIndices.push_back(stoi(nums[5]));

			vertexIndices.push_back(stoi(nums[6]));
			uvIndices.push_back(stoi(nums[7]));
			normalIndices.push_back(stoi(nums[8]));
		}
	}

	for (int v : vertexIndices)
		vertices.push_back(temp_vertices[v - 1]);

	for (int u : uvIndices)
		uvs.push_back(temp_uvs[u - 1]);

	for (int n : normalIndices)
		normals.push_back(temp_normals[n - 1]);
}