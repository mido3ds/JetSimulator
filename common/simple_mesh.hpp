#ifndef SIMPLE_MESH_HPP
#define SIMPLE_MESH_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "vertex.h"

class SimpleMesh {
private:
    //VAO: Vertex Array Object
    //VBO, VBO2: Vertex Buffer Objects (VBO2 is only used sometimes)
    //VertexCount: store number of vertices
    GLuint VAO, VBO, VBO2, VertexCount;

    //Delete copy constructor and assignment operation
    SimpleMesh(SimpleMesh const &) = delete;
    SimpleMesh & operator =(SimpleMesh const &) = delete;

public:

    SimpleMesh();
    ~SimpleMesh();

    //Position only vertices bound to a specific location
    void setup(GLuint attribLoc, const std::vector<GLfloat> & vertices);

    //Position only vertices (Assuming position is sent to location 0)
    void setup(const std::vector<GLfloat> &vertices);

    //Positions and colors in separate buffers
    //(Assuming position is sent to location 0 and Color is sent to location 1)
    void setupWithColors(const std::vector<GLfloat> &vertices, const std::vector<GLfloat> &colors);

    //Positions and colors in separate buffers where colors are defined using unsigned bytes
    //(Assuming position is sent to location 0 and Color is sent to location 1)
    void setupWithColors8bit(const std::vector<GLfloat> &vertices, const std::vector<GLubyte> &colors);
    
    //Positions and colors in separate buffers (BATCHED)
    //(Assuming position is sent to location 0 and Color is sent to location 1)
    void setupWithColorsBatched(const std::vector<GLfloat> &vertices, const std::vector<GLfloat> &colors);

    //Positions and colors in one buffers (INTERLEAVED)
    //(Assuming position is sent to location 0 and Color is sent to location 1)
    void setupWithColorsInterleaved(const std::vector<GLfloat> &vertices);

    //Position and colors in one buffer (INTERLEAVED) but data is defined in a struct
    //(Assuming position is sent to location 0 and Color is sent to location 1)
    void setupWithColorsInterleaved(const std::vector<Vertex> &vertices);

    //Render mesh
    void draw();

};

#endif