#include "Shader.hpp"
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

void Shader::attach(std::string const &filename, GLenum type) {
    std::ifstream file(filename.c_str());
    if (file.fail()) {
        std::cerr << "UNABLE TO OPEN FILE: \"" << filename << "\"\n";
        return;
    }
    std::string source = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

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
        std::cerr << "ERROR IN " << filename << std::endl;
        std::cerr << logStr << std::endl;
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
        std::cerr << "LINKING ERROR" << std::endl;
        std::cerr << logStr << std::endl;
        delete[] logStr;
    }
    assert(status);
}