#pragma once
#include "Texture.hpp"
#include <string>
#include <cstdint>

using namespace std;

class Texture2D: public Texture {
public:
    enum class Type: uint8_t {
        Diffuse, Specular
    };

    Texture2D(const string& path, Type type);

    virtual void load() override;
private:
    Type type;
    const string path;
};
