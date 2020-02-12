#pragma once
#include <glad/glad.h>
#include <string>

class Image {
public:
    const std::string path;
    unsigned char* data = nullptr;
    int width, height, channels;
    GLuint glInternalFormat, glFormat;

    Image(const std::string& path);
    ~Image();
    
    void load();
};