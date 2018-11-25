#include "Mesh.hpp"
using namespace std;

Mesh::Mesh(vector<glm::vec3> *positions, 
        vector<glm::vec3> *normals, 
        vector<glm::vec3> *textCoords,
        vector<glm::ivec3> *indices,
        Material *material) 
    :positions(positions), normals(normals), 
    textCoords(textCoords), indices(indices), material(material) {
    elementCount = indices.size();
}

Mesh::~Mesh() {
    if (isLoaded()) {
        glBindVertexArray(vao);
        glDeleteBuffers(1, &ebo);
        glDeleteBuffers(3, vbo);
        glBindVertexArray(0);

        glDeleteVertexArrays(1, &vao);
        delete vbo;
        vao = 0;
    }
}

void Mesh::load() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(3, vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, positions->size() * sizeof(glm::vec3), positions->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(glm::vec3), normals->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, textCoords->size() * sizeof(glm::vec3), textCoords->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(glm::ivec3), indices->data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    delete positions;
    delete normals;
    delete textCoords;
    delete indices;
    positions = normals = textCoords = indices = nullptr;
}

bool Mesh::isLoaded() {
    return vao;
}

void Mesh::draw() {
    if (!isLoaded()) return;

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}