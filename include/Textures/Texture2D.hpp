#pragma once
#include "Texture.hpp"
#include <string>

using namespace std;

class Texture2D: public Texture {
public:
    enum Usage {
        Diffuse, Specular
    };

    Texture2D(const string& path, Usage usage);

    virtual void load() override;
private:
    Usage usage;
    const string path;
};