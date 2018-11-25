#pragma once
#include <string>

class Texture {
public:
    enum Type {
        Diffuse, Specular
    };

    Texture(std::string *path, Texture::Type type);
    ~Texture();

    void load();
    bool isLoaded();
    void bind();

private:
    std::string *path;
    GLuint texId = 0;
    Type type;
};