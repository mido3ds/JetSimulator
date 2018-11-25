#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "vertex.h"

class Mesh {
private:
    //VAO: Vertex Array Object
    //VBO: Vertex Buffer Object
    //EBO: Elements Buffer Object
    //ElementCount: store number of vertices
    GLuint VAO, VBO, EBO, ElementCount;

    //Delete copy constructor and assignment operation
    Mesh(Mesh const &) = delete;
    Mesh & operator =(Mesh const &) = delete;

public:
    Mesh();
    ~Mesh();
    void setup(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);    
    void draw();
};

#endif