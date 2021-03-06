#include <Textures/Texture2D.hpp>
#include <Textures/Image.hpp>

Texture2D::Texture2D(const string& path, Type type) 
    :Texture(GL_TEXTURE_2D, GLuint(type)), path(path) {}

void Texture2D::load() {
    glGenTextures(1, &texId);
    bind();

    Image image(path);
    image.load();

    glTexImage2D(GL_TEXTURE_2D, 0, image.glInternalFormat, image.width, 
        image.height, 0, image.glFormat, GL_UNSIGNED_BYTE, &image.data.value().get());
    glGenerateMipmap(GL_TEXTURE_2D);
}
