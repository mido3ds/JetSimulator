#pragma once
#include <glad/glad.h>
#include <string>

using namespace std;

class Image {
public:
    const string path;
    unsigned char* data = nullptr;
    int width, height, channels;
    GLuint glInternalFormat, glFormat;

    Image(const string& path);
    ~Image();
    
    void load();
};