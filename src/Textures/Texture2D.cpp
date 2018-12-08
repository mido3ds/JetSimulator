#include "Texture2D.hpp"

Texture2D::Texture2D(const std::string& path, Usage usage) 
    :Texture(GL_TEXTURE_2D, usage, path) {}