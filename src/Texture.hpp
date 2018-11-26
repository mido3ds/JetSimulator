#pragma once
#include <string>
#include <glad/glad.h>

class Texture {
public:
    enum Type {
        Diffuse, Specular
    };

    Texture(const std::string& path, Texture::Type type);
    ~Texture();

    void load();
    bool isLoaded() const;
    void bind() const;

private:
    std::string path;
    GLuint texId = 0;
    Type type;
};