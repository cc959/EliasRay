#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "GlStorage.h"

using namespace std;

#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;

class Object;

class Shader : GlStorage
{
public:
    GLuint ProgramID = 0;

    map<string, GLuint> uniforms;

    Shader();

    Shader(const string &vertexPath, const string &fragmentPath);

    Shader(Shader &&other);
    Shader(const Shader &other);
    Shader &operator=(Shader &&other);
    Shader &operator=(const Shader &other);

    operator GLuint() const;

    bool ImportShader(const string &vertexPath, const string &fragmentPath);

    bool BindShader();

    ~Shader();

private:
    bool Release() override;

    template <class U>
    void Copy(U &&other);
};
