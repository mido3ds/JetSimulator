#pragma once
#include <glad/glad.h>
#include <string>

class Shader {
private:
    GLuint programID;

    Shader(Shader const &) = delete;
    Shader & operator =(Shader const &) = delete;
public:
    Shader();
    ~Shader();

    void use();
    GLuint getID(); 
    void attach(std::string const &filename, GLenum type);
    void link();
};