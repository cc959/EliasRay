#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <GlStorage.h>

using namespace std;

class ComputeShader : GlStorage
{
public:
	GLuint ProgramID = 0;

	ComputeShader();

	ComputeShader(const string &path);

	ComputeShader(ComputeShader &&other);
	ComputeShader(const ComputeShader &other);
	ComputeShader &operator=(ComputeShader &&other);
	ComputeShader &operator=(const ComputeShader &other);

	operator GLuint() const;

	bool ImportShader(const std::string &path);

	bool BindShader();

	~ComputeShader();

private:
	bool Release() override;

	template <class U>
	void Copy(U &&other);
};
