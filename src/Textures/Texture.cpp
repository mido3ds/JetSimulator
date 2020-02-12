#include "Textures/Texture.hpp"
#include <glad/glad.h>

Texture::Texture(GLint type, GLuint unit) 
    :type(type), unit(unit) {}

Texture::~Texture() {
    if (isLoaded()) {
        glDeleteTextures(1, &texId);
    }
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
