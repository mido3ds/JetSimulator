#include <Scene/Mesh.hpp>
#include <cassert>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

shared_ptr<Mesh> Mesh::build(aiMesh* mesh, shared_ptr<const Material> material) {
    assert(mesh->HasFaces() && mesh->HasNormals() && mesh->HasPositions());

    auto positions = vector<glm::vec3>(mesh->mNumVertices), 
                            normals = vector<glm::vec3>(mesh->mNumVertices);
    auto textCoords = vector<glm::vec2>(mesh->mNumVertices);
    auto indices = vector<glm::ivec3>(mesh->mNumFaces);

    for (int i = 0; i < positions.size(); i++) {
        positions[i] = glm::make_vec3(&mesh->mVertices[i].x);
        normals[i] = glm::make_vec3(&mesh->mNormals[i].x);

        if (mesh->HasTextureCoords(0)) {
            textCoords[i] = glm::make_vec2(&mesh->mTextureCoords[0][i].x);
        }
    }

    for (int i = 0; i < indices.size(); i++) {
        indices[i] = glm::ivec3(mesh->mFaces[i].mIndices[0], 
            mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]);
    }

    return make_shared<Mesh>(positions, normals, textCoords, indices, move(material));
}

Mesh::Mesh(vector<glm::vec3> positions, 
        vector<glm::vec3> normals, 
        vector<glm::vec2> textCoords,
        vector<glm::ivec3> indices,
        shared_ptr<const Material> material) 
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
    }
}

void Mesh::load() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(3, vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
  
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, textCoords.size() * sizeof(glm::vec2), textCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::ivec3), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    positions.clear();
    normals.clear();
    textCoords.clear();
    indices.clear();
}

bool Mesh::isLoaded() {
    return vao;
}

void Mesh::render(PhongShader& shader) {
    shader.setMaterial(material);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, elementCount * 3, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
