#include <Shaders/Shader.hpp>
#include <cassert>
#include <iostream>
#include <fstream>

Shader::Shader() { 
    programID = glCreateProgram(); 
}

Shader::~Shader() { 
    glDeleteProgram(programID); 
}

void Shader::use() { 
    glUseProgram(programID); 
}

GLuint Shader::getID() {
    return programID; 
}

void Shader::attach(string const &filename, GLenum type) {
    ifstream file(filename.c_str());
    if (file.fail()) {
        cerr << "UNABLE TO OPEN FILE: \"" << filename << "\"\n";
        return;
    }
    string source = string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());

    GLuint shaderID = glCreateShader(type); 

    const char * sourceCharArray = source.c_str();
    glShaderSource(shaderID, 1, &sourceCharArray, nullptr); 
    glCompileShader(shaderID); 

    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if(!status) {
        GLint length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char* logStr = new char[length];
        glGetShaderInfoLog(shaderID, length, nullptr, logStr);
        cerr << "ERROR IN " << filename << endl;
        cerr << logStr << endl;
        delete[] logStr;
    }
    assert(status);

    glAttachShader(programID, shaderID); 
    glDeleteShader(shaderID); 
}

void Shader::link() {
    glLinkProgram(programID);

    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);
    if (!status) {
        GLint length;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
        char* logStr = new char[length];
        glGetProgramInfoLog(programID, length, nullptr, logStr);
        cerr << "LINKING ERROR" << endl;
        cerr << logStr << endl;
        delete[] logStr;
    }
    assert(status);
}

GLuint Shader::getUniformLocation(string uniform) {
    return glGetUniformLocation(programID, uniform.c_str());
}

void Shader::setUniform(int loc, int v) {
    glUniform1i(loc, v);
}

void Shader::setUniform(int loc, float v) {
     glUniform1f(loc, v);
}

void Shader::setUniform(int loc, glm::mat4 mat) {
     glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniform(int loc, glm::mat3 mat) {
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniform(int loc, glm::vec4 v) {
    glUniform4fv(loc, 1, glm::value_ptr(v));
}

void Shader::setUniform(int loc, glm::vec3 v) {
    glUniform3fv(loc, 1, glm::value_ptr(v));
}

void Shader::setUniform(int loc, glm::vec2 v) {
    glUniform2fv(loc, 1, glm::value_ptr(v));
}

void Shader::setUniform(int loc, glm::ivec4 v) {
    glUniform4iv(loc, 1, glm::value_ptr(v));
}

void Shader::setUniform(int loc, glm::ivec3 v) {
    glUniform3iv(loc, 1, glm::value_ptr(v));
}

void Shader::setUniform(int loc, glm::ivec2 v) {
    glUniform2iv(loc, 1, glm::value_ptr(v));
}
