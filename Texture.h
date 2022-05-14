#pragma once

#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "GlStorage.h"
#include "Mesh.h"

using namespace std;
using namespace glm;

class Texture : GlStorage
{
public:
    GLuint textureID = 0;

    Texture();
    Texture(const std::string &path);
    Texture(unsigned width, unsigned height, GLenum internalFormat, GLenum format, const void *pixels);

    Texture(Texture &&other);
    Texture(const Texture &other);
    Texture &operator=(Texture &&other);
    Texture &operator=(const Texture &other);

    operator GLuint() const;

    void Create(unsigned width, unsigned height, GLenum internalFormat, GLenum format, const void *pixels);
    void SetFilters(GLint MinFilter, GLint MagFilter);
    bool LoadFromFile(const string path);
    void BindTexture();

    ~Texture();

private:
    template <class U>
    void Copy(U &&);

    bool Release();
};
