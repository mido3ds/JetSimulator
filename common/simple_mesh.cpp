#include "simple_mesh.hpp"

SimpleMesh::SimpleMesh(){
    VAO = VBO = VBO2 = (GLuint)0;
    VertexCount = 0;
}

SimpleMesh::~SimpleMesh(){
    //Delete Arrays and Buffers if they were created
    if(VAO != 0) glDeleteVertexArrays(1, &VAO);
    if(VBO != 0) glDeleteBuffers(1, &VBO);
    if(VBO2 != 0) glDeleteBuffers(1, &VBO2);
}

void SimpleMesh::draw(){
    if(VAO == 0) return;
    //Bind Vertex Array
    glBindVertexArray(VAO);
    //Draw all vertex as triangles (each 3 vertices will be a triangle)
    glDrawArrays(GL_TRIANGLES, 0, VertexCount);
    //Unbind Vertex Array
    glBindVertexArray(0);
}

/*
*
* Position only vertices bound to a specific location
*
*/
void SimpleMesh::setup(GLuint attribLoc, const std::vector<GLfloat> & vertices){
    //Get number of vertices (each vertex is 3 floats [x,y,z])
    VertexCount = vertices.size() / 3;

    glGenVertexArrays(1, &VAO); //Generate Vertex Arrays
    glGenBuffers(1, &VBO); //Generate Vertex Buffer

    //Bind Vertex Array
    //The VAO will store all the "glVertexAttribPointer" and "glEnableVertexAttribArray" till it is unbound
    glBindVertexArray(VAO);

    //Bind Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Fill Buffer with data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    //Define how the buffer should be read and sent to the shader
    //Arguments:
    //  index: which attribute location should receive this data
    //  size: number of elements to send
    //  type: type of elements to send
    //  normalized: should data be normalized (e.g. bytes will be normalized from [-128,127] to [-1,1])
    //  stride: how much to increment the pointer after reading a value
    //  offset: the initial value of the pointer before reading any value
    //In this case, Attribute will read data as 3 floats without normalization
    //The pointer will increment 12 bytes (3*sizeof(float)) and will start at 0
    glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    //Enable Attribute location
    glEnableVertexAttribArray(attribLoc);

    //Unbind Vertex Array so that it doesn't store any other operation
    glBindVertexArray(0);
}

/*
*
* Position only vertices (Assuming position is sent to location 0)
*
*/
void SimpleMesh::setup(const std::vector<GLfloat> & vertices){
    //Get number of vertices (each vertex is 3 floats [x,y,z])
    VertexCount = vertices.size() / 3;

    glGenVertexArrays(1, &VAO); //Generate Vertex Arrays
    glGenBuffers(1, &VBO); //Generate Vertex Buffer

    //Bind Vertex Array
    //The VAO will store all the "glVertexAttribPointer" and "glEnableVertexAttribArray" till it is unbound
    glBindVertexArray(VAO);

    //Bind Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Fill Buffer with data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    //Define how the buffer should be read and sent to the shader
    //Arguments:
    //  index: which attribute location should receive this data
    //  size: number of elements to send
    //  type: type of elements to send
    //  normalized: should data be normalized (e.g. bytes will be normalized from [-128,127] to [-1,1])
    //  stride: how much to increment the pointer after reading a value
    //  offset: the initial value of the pointer before reading any value
    //In this case, Attribute location 0 will read data as 3 floats without normalization
    //The pointer will increment 12 bytes (3*sizeof(float)) and will start at 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    //Enable Attribute location 0
    glEnableVertexAttribArray(0);

    //Unbind Vertex Array so that it doesn't store any other operation
    glBindVertexArray(0);
}

/*
*
* Positions and colors in separate buffers
* (Assuming position is sent to location 0 and Color is sent to location 1)
*
*/
void SimpleMesh::setupWithColors(const std::vector<GLfloat> &vertices, const std::vector<GLfloat> &colors){
    //Get number of vertices (each vertex position is 3 floats [x,y,z])
    VertexCount = vertices.size() / 3;
    //Check that we have enough color data (each color is 4 floats [r,g,b,a])
    if(colors.size() != VertexCount * 4) {
        VertexCount = 0;
        return;
    }

    //Generate one vertex array and 2 vertex buffers (1 for positions and 1 for colors)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);

    //Bind Vertex Array
    //The VAO will store all the "glVertexAttribPointer" and "glEnableVertexAttribArray" till it is unbound
    glBindVertexArray(VAO);

    //Bind 1st buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Fill with position data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    //Enable and point attribute location 0 to 1st buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    //Bind 2nd Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    //Fill with color data
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

    //Enable and point attribute location 1 to 2nd buffer
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);

    //Unbind Vertex Array so that it doesn't store any other operation
    glBindVertexArray(0);
}

/*
*
* Positions and colors in separate buffers where colors are defined using unsigned bytes
* (Assuming position is sent to location 0 and Color is sent to location 1)
*
*/
void SimpleMesh::setupWithColors8bit(const std::vector<GLfloat> &vertices, const std::vector<GLubyte> &colors){
    //Get number of vertices (each vertex position is 3 floats [x,y,z])
    VertexCount = vertices.size() / 3;
    //Check that we have enough color data (each color is 4 floats [r,g,b,a])
    if(colors.size() != VertexCount * 4) {
        VertexCount = 0;
        return;
    }

    //Generate one vertex array and 2 vertex buffers (1 for positions and 1 for colors)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);

    //Bind Vertex Array
    //The VAO will store all the "glVertexAttribPointer" and "glEnableVertexAttribArray" till it is unbound
    glBindVertexArray(VAO);

    //Bind 1st buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Fill with position data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    //Enable and point attribute location 0 to 1st buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    //Bind 2nd Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    //Fill with color data
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLubyte), colors.data(), GL_STATIC_DRAW);

    //Enable and point attribute location 1 to 2nd buffer
    //In this case we set "normalized" to true since we want to map the ubyte range [0,255] to float [0,1]
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 4 * sizeof(GLubyte), (void*)0);
    glEnableVertexAttribArray(1);

    //Unbind Vertex Array so that it doesn't store any other operation
    glBindVertexArray(0);
}

/*
*
* Positions and colors in separate buffers (BATCHED)
* (Assuming position is sent to location 0 and Color is sent to location 1)
*
*/
void SimpleMesh::setupWithColorsBatched(const std::vector<GLfloat> &vertices, const std::vector<GLfloat> &colors){
    //Get number of vertices (each vertex position is 3 floats [x,y,z])
    VertexCount = vertices.size() / 3;
    //Check that we have enough color data (each color is 4 floats [r,g,b,a])
    if(colors.size() != VertexCount * 4) {
        VertexCount = 0;
        return;
    }

    //Generate one vertex array and 2 vertex buffers (1 for positions and 1 for colors)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Bind Vertex Array
    //The VAO will store all the "glVertexAttribPointer" and "glEnableVertexAttribArray" till it is unbound
    glBindVertexArray(VAO);

    //Bind 1st buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Allocate empty buffer for position and color data
    glBufferData(GL_ARRAY_BUFFER, (vertices.size() + colors.size()) * sizeof(GLfloat) , nullptr, GL_STATIC_DRAW);
    //Send position data to buffer starting from offset 0
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());
    //Send color data to buffer starting from after the position data
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), colors.size() * sizeof(GLfloat), colors.data());

    //Enable and point attribute location 0 to the buffer starting from offset 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    //Enable and point attribute location 1 to the buffer starting from after the vertices
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(vertices.size() * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //Unbind Vertex Array so that it doesn't store any other operation
    glBindVertexArray(0);
}

/*
*
* Positions and colors in one buffers (INTERLEAVED)
* (Assuming position is sent to location 0 and Color is sent to location 1)
*
*/
void SimpleMesh::setupWithColorsInterleaved(const std::vector<GLfloat> &vertices){
    //Get number of vertices (each vertex is 7 floats: 3 for position [x,y,z] + 4 for color [r,g,b,a])
    VertexCount = vertices.size() / 7;

    //Generate a Vertex Array and a Buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Bind Vertex Array
    //The VAO will store all the "glVertexAttribPointer" and "glEnableVertexAttribArray" till it is unbound
    glBindVertexArray(VAO);

    //Bind buffer and fill with data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    //For position data
    //Attribute location 0 will read data as 3 floats without normalization
    //The pointer will increment 28 bytes (7*sizeof(float)) and will start at 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    //For color data
    //Attribute location 1 will read data as 4 floats without normalization
    //The pointer will increment 28 bytes (7*sizeof(float)) and will start after 12 bytes (after the first position)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //Unbind Vertex Array so that it doesn't store any other operation
    glBindVertexArray(0);
}

/*
*
* Positions and colors in one buffers (INTERLEAVED) but data is defined in a struct
* (Assuming position is sent to location 0 and Color is sent to location 1)
*
*/
void SimpleMesh::setupWithColorsInterleaved(const std::vector<Vertex> &vertices){
    //Get number of vertices
    VertexCount = vertices.size();

    //Generate a Vertex Array and a Buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Bind Vertex Array
    //The VAO will store all the "glVertexAttribPointer" and "glEnableVertexAttribArray" till it is unbound
    glBindVertexArray(VAO);
    
    //Bind buffer and fill with data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

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
    
    //Unbind Vertex Array so that it doesn't store any other operation
    glBindVertexArray(0);
}