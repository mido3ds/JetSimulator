#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include "Material.hpp"
#include <Shaders/PhongShader.hpp>
#include <memory>

using namespace std;

class Mesh {
private:
    GLuint vao = 0, vbo[3] = {0,0,0}, ebo = 0, elementCount = 0;

    vector<glm::vec3> positions, normals;
    vector<glm::vec2> textCoords;
    vector<glm::ivec3> indices;

    Mesh & operator =(Mesh const &) = delete;
public:
    static shared_ptr<Mesh> build(aiMesh* mesh, shared_ptr<const Material> material);

    Mesh(vector<glm::vec3> positions, 
        vector<glm::vec3> normals, 
        vector<glm::vec2> textCoords,
        vector<glm::ivec3> indices,
        shared_ptr<const Material> material);
    ~Mesh();

    void load();
    bool isLoaded();
    void render(unique_ptr<PhongShader>& shader);

    shared_ptr<const Material> material;
};
