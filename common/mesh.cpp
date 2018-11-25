#include "mesh.hpp"

Mesh::Mesh(){
    VAO = VBO = EBO = (GLuint)0;
    ElementCount = 0;
}

Mesh::~Mesh(){
    //Delete Arrays and Buffers if they were created
    if(VAO != 0) glDeleteVertexArrays(1, &VAO);
    if(VBO != 0) glDeleteBuffers(1, &VBO);
    if(EBO != 0) glDeleteBuffers(1, &EBO);
}

void Mesh::draw(){
    if(VAO == 0) return;
    //Bind Vertex Array
    glBindVertexArray(VAO);
    //DrawElements uses the element buffer to create triangles to be drawn
    glDrawElements(GL_TRIANGLES, ElementCount, GL_UNSIGNED_INT, (void*)0);
    //Unbind Vertex Array
    glBindVertexArray(0);
}


void Mesh::setup(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices){
    //Get number of elements
    ElementCount = indices.size();

    //Generate 1 Vertex Array and 2 Buffers (1 Vertex buffer + 1 Element Buffer)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Bind Vertex Array
    //The VAO will store all the "glVertexAttribPointer" and "glEnableVertexAttribArray" till it is unbound
    //It will also store all the "glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ...)"
    glBindVertexArray(VAO);
    
    //Bind and fill vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    //Define how the buffer should be read and sent to the shader
    //Arguments:
    //  index: which attribute location should receive this data
    //  size: number of elements to send
    //  type: type of elements to send
    //  normalized: should data be normalized (e.g. bytes will be normalized from [-128,127] to [-1,1])
    //  stride: how much to increment the pointer after reading a value
    //  offset: the initial value of the pointer before reading any value

    //For position data
    //Attribute location 0 will read data as 3 floats without normalization
    //The pointer will increment 28 bytes (sizeof(Vertex)) and will start at offset of position in Vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    //For color data
    //Attribute location 1 will read data as 4 floats without normalization
    //The pointer will increment 28 bytes (sizeof(Vertex)) and will start at offset of color in Vertex
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    //Bind and fill element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    //Unbind Vertex Array so that it doesn't store any other operation
    glBindVertexArray(0);
} 