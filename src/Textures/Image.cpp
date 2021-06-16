#include <Textures/Image.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(const string& path) :path(path) {}

Image::~Image() {
    stbi_image_free(data);
    data = nullptr;
}

void Image::load() {
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    
    switch (channels) {
        case 1: glInternalFormat = GL_R8; glFormat = GL_RED; break;
        case 2: glInternalFormat = GL_RG8; glFormat = GL_RG; break;
        case 3: glInternalFormat = GL_RGB8; glFormat = GL_RGB; break;
        case 4: glInternalFormat = GL_RGBA8; glFormat = GL_RGBA; break;
        default: return; // TODO print error
    }
}