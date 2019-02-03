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
    void attach(const std::string& source, GLenum type);
    void attachFile(const std::string& filename, GLenum type);
    void link();

    GLuint getUniformLocation(std::string uniform);
    void setUniform(int loc, const int& v);
    void setUniform(int loc, const float& v);
    void setUniform(int loc, const glm::mat4& mat);
    void setUniform(int loc, const glm::mat3& mat);
    void setUniform(int loc, const glm::vec4& v);
    void setUniform(int loc, const glm::vec3& v);
    void setUniform(int loc, const glm::vec2& v);
    void setUniform(int loc, const glm::ivec4& v);
    void setUniform(int loc, const glm::ivec3& v);
    void setUniform(int loc, const glm::ivec2& v);
};