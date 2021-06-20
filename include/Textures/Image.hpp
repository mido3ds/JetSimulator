#pragma once
#include <glad/glad.h>
#include <string>
#include <optional>
#include <functional>
#include <cstdint>

using namespace std;

class Image {
    void _free();
public:
    const string path;
    optional<reference_wrapper<uint8_t>> data = {};
    int width, height, channels;
    GLuint glInternalFormat, glFormat;

    Image(const string& path);
    ~Image();
    
    void load();
};
