#include "Texture.hpp"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(GLint type, GLuint unit, const std::string& path) 
    :path(path), type(type), unit(unit) {}

Texture::~Texture() {
    if (isLoaded()) {
        glDeleteTextures(1, &texId);
    }
}

void Texture::load() {
    glGenTextures(1, &texId);
    bind();

    int width, height, nrChannels;
    unsigned char* data;

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    
    GLint internalFormat, format;
    switch (nrChannels) {
        case 1: internalFormat = GL_R8; format = GL_RED; break;
        case 2: internalFormat = GL_RG8; format = GL_RG; break;
        case 3: internalFormat = GL_RGB8; format = GL_RGB; break;
        case 4: internalFormat = GL_RGBA8; format = GL_RGBA; break;
        default: return; // TODO print error
    }

    glTexImage2D(type, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(type);

    stbi_image_free(data);
}

bool Texture::isLoaded() const {
    return texId;
}

void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0+unit);
    glBindTexture(type, texId);
}

void Texture::setParameter(GLenum pname, GLint param) {
    glTexParameteri(type, pname, param);
}

GLint Texture::getParameter(GLenum pname) {
    GLint result;
    glGetTexParameteriv(type, pname, &result);
    return result;
}
