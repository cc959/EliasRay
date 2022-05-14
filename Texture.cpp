#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture(const string &path) : Texture()
{
    LoadFromFile(path);
}

Texture::Texture(unsigned width, unsigned height, GLenum internalFormat, GLenum format, const void *pixels)
{
    Create(width, height, internalFormat, format, pixels);
}

void Texture::Create(unsigned width, unsigned height, GLenum internalFormat, GLenum format, const void *pixels)
{
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
}
bool Texture::LoadFromFile(const std::string path)
{
    try
    {
        sf::Image image;
        image.loadFromFile(path);
        image.flipVertically();
        // Give the image to OpenGL
        Create(image.getSize().x, image.getSize().y, GL_RGBA, GL_RGBA, image.getPixelsPtr());
        std::cout << "Loaded \"" << path << "\" successfully\n";
    }
    catch (int e)
    {
        std::cout << "Error while loading Texture from \"" << path << "\" Maybe wrong file path? Error code: " << e << "\n";
        return false;
    }

    return true;
}
void Texture::SetFilters(GLint MinFilter, GLint MagFilter)
{
    // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, MagFilter);
    // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, MinFilter);
    if (MinFilter == GL_NEAREST_MIPMAP_NEAREST || MinFilter == GL_LINEAR_MIPMAP_NEAREST || MinFilter == GL_NEAREST_MIPMAP_LINEAR || MinFilter == GL_LINEAR_MIPMAP_LINEAR)
    {
        // Generate mipmaps, by the way.
        glGenerateTextureMipmap(textureID);
    }
}
void Texture::BindTexture()
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture::operator GLuint() const
{
    return textureID;
}

template <class U>
void Texture::Copy(U &&other)
{
    if (this != &other)
    {
        Release();

        GlStorage::Copy(other.id);
        textureID = other.textureID;
    }
}

Texture::Texture(Texture &&other)
{
    Copy(other);
}

Texture::Texture(const Texture &other)
{
    Copy(other);
}

Texture &Texture::operator=(Texture &&other)
{
    Copy(other);
    return *this;
}

Texture &Texture::operator=(const Texture &other)
{
    Copy(other);
    return *this;
}

bool Texture::Release()
{
    if (GlStorage::Release())
    {
        glDeleteTextures(1, &textureID);
        return true;
    }
    return false;
}

Texture::~Texture()
{
    Release();
}