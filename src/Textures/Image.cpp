#include <Textures/Image.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(const string& path) :path(path) {}

void Image::_free() {
    stbi_image_free(&data.value().get());
    data = {};
}

Image::~Image() {
    if (data) {
        _free();
    }
}

void Image::load() {
    stbi_set_flip_vertically_on_load(true);
    auto dataptr = stbi_load(path.c_str(), &width, &height, &channels, 0);
    
    if (data) {
        _free();
    }
    data = {*dataptr};
    
    switch (channels) {
        case 1: glInternalFormat = GL_R8; glFormat = GL_RED; break;
        case 2: glInternalFormat = GL_RG8; glFormat = GL_RG; break;
        case 3: glInternalFormat = GL_RGB8; glFormat = GL_RGB; break;
        case 4: glInternalFormat = GL_RGBA8; glFormat = GL_RGBA; break;
        default: return; // TODO print error
    }
}
