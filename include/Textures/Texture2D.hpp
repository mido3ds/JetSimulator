#pragma once
#include "Texture.hpp"
#include <string>

class Texture2D: public Texture {
public:
    enum Usage {
        Diffuse, Specular
    };

    Texture2D(const std::string& path, Usage usage);

    virtual void load() override;
private:
    Usage usage;
    const std::string path;
};