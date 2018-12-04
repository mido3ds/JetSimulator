#pragma once
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
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

    GLuint getUniformLocation(std::string uniform);
    void setUniform(int loc, int v);
    void setUniform(int loc, float v);
    void setUniform(int loc, glm::mat4 mat);
    void setUniform(int loc, glm::mat3 mat);
    void setUniform(int loc, glm::vec4 v);
    void setUniform(int loc, glm::vec3 v);
    void setUniform(int loc, glm::vec2 v);
    void setUniform(int loc, glm::ivec4 v);
    void setUniform(int loc, glm::ivec3 v);
    void setUniform(int loc, glm::ivec2 v);
};