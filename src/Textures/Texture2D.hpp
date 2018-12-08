#pragma once
#include "Texture.hpp"

class Texture2D: public Texture {
public:
    enum Usage {
        Diffuse, Specular
    };

    Texture2D(const std::string& path, Usage usage);
private:
    Usage usage;
};