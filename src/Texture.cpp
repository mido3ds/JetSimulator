#include "Texture.hpp"
#include <glad/glad.h>
#include <stb_image.h>

Texture::Texture(std::string *path, Type type) :path(path), type(type) {}

Texture::~Texture() {
    if (isLoaded()) {
        glDeleteTextures(1, &texId);
        texId = 0;
    }
}

void Texture::load() {
    glGenTextures(1, &texId);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	//todo add ability to configure
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path->c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    path = 0;
}

bool Texture::isLoaded() {
    return texId;
}

void Texture::bind() {
    glActiveTexture(GL_TEXTURE0+type);
    glBindTexture(GL_TEXTURE_2D, texId);
}
