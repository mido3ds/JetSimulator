#include "Texture2D.hpp"
#include "Image.hpp"

Texture2D::Texture2D(const std::string& path, Usage usage) 
    :Texture(GL_TEXTURE_2D, usage), path(path) {}

void Texture2D::load() {
    glGenTextures(1, &texId);
    bind();

    Image image(path);
    image.load();

    glTexImage2D(type, 0, image.glInternalFormat, image.width, 
        image.height, 0, image.glFormat, GL_UNSIGNED_BYTE, image.data);
    glGenerateMipmap(type);
}