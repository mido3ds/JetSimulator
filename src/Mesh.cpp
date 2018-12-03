#include "Mesh.hpp"
#include <cassert>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

Mesh* Mesh::build(aiMesh* mesh, const Material* material) {
    assert(mesh->HasFaces() && mesh->HasNormals() && mesh->HasPositions());

    std::vector<glm::vec3> *positions = new std::vector<glm::vec3>(mesh->mNumVertices), 
                            *normals = new std::vector<glm::vec3>(mesh->mNumVertices);
    std::vector<glm::vec2> *textCoords = new std::vector<glm::vec2>(mesh->mNumVertices);
    std::vector<glm::ivec3> *indices = new std::vector<glm::ivec3>(mesh->mNumFaces);

    for (int i = 0; i < positions->size(); i++) {
        positions[0][i] = glm::make_vec3(&mesh->mVertices[i].x);
        normals[0][i] = glm::make_vec3(&mesh->mNormals[i].x);

        if (mesh->HasTextureCoords(0)) {
            textCoords[0][i] = glm::make_vec2(&mesh->mTextureCoords[0][i].x);
        }
    }

    for (int i = 0; i < indices->size(); i++) {
        indices[0][i] = glm::ivec3(mesh->mFaces[i].mIndices[0], 
            mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]);
    }

    return new Mesh(positions, normals, textCoords, indices, material);
}

Mesh::Mesh(vector<glm::vec3> *positions, 
        vector<glm::vec3> *normals, 
        vector<glm::vec2> *textCoords,
        vector<glm::ivec3> *indices,
        const Material *material) 
    :positions(positions), normals(normals), 
    textCoords(textCoords), indices(indices), material(material) {
    elementCount = indices->size();
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
    glBufferData(GL_ARRAY_BUFFER, textCoords->size() * sizeof(glm::vec2), textCoords->data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(glm::ivec3), indices->data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    delete positions;
    delete normals;
    delete textCoords;
    delete indices;
    positions = normals = nullptr; 
    textCoords = nullptr;
    indices = nullptr;
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