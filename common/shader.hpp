#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

class Shader {

private:
    //Shader Program Handle
    GLuint programID;

    //Delete copy constructor and assignment operation
    Shader(Shader const &) = delete;
    Shader & operator =(Shader const &) = delete;

public:

    //Create Shader Program
    Shader() { programID = glCreateProgram(); }
    
    //Delete Shader Program
    ~Shader() { glDeleteProgram(programID); }

    //Bind shader program in order to be used
    void use() { glUseProgram(programID); }

    //Get Program Handle
    GLuint getID() { return programID; }

    //Read shader from file, send it to GPU, compile it then attach it to shader
    void attach(std::string const &filename, GLenum type){

        std::ifstream file(filename.c_str());
        if (file.fail())
        {
            std::cerr << "UNABLE TO OPEN FILE: \"" << filename << "\"\n";
            return;
        }
        std::string source = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

        GLuint shaderID = glCreateShader(type); //Create shader of the given type

        const char * sourceCharArray = source.c_str();
        glShaderSource(shaderID, 1, &sourceCharArray, nullptr); //Send shader source code
        glCompileShader(shaderID); //Compile the shader code

        //Check and log for any error in the compilation process
        GLint status;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
        if(!status){
            GLint length;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
            char* logStr = new char[length];
            glGetShaderInfoLog(shaderID, length, nullptr, logStr);
            std::cerr << "ERROR IN " << filename << std::endl;
            std::cerr << logStr << std::endl;
            delete[] logStr;
        }
        assert(status);


        glAttachShader(programID, shaderID); //Attach shader to program
        glDeleteShader(shaderID); //Delete shader (the shader is already attached to the program so its object is no longer needed)
    }

    //Link Program (Do this after all shaders are attached)
    void link()
    {
        //Link
        glLinkProgram(programID);

        //Check and log for any error in the linking process
        GLint status;
        glGetProgramiv(programID, GL_LINK_STATUS, &status);
        if (!status)
        {
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

};

#endif