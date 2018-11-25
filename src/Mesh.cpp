#include "Mesh.hpp"
#include <cassert>
using namespace std;

static Mesh* Mesh::build(const aiScene* scene, const aiMesh* mesh) {
    assert(scene->HasFaces() && scene->HasNormals() && scene->HasPositions() && scene->HasTextureCoords());

    std::vector<glm::vec3> *positions = new std::vector<glm::vec3>(mesh->mNumVertices), 
                            *normals = new std::vector<glm::vec3>(mesh->mNumVertices), 
                            *textCoords = new std::vector<glm::vec3>(mesh->mNumVertices);
    std::vector<glm::ivec3> *indices = new std::vector<glm::ivec3>(mesh->mNumFaces);

    aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];

    // for (int i = 0; i < positions->size(); i++) {
    //     positions[i] = glm::make_vec3(mesh->mVertices[])
    // }
    return nullptr;//TODO
}

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