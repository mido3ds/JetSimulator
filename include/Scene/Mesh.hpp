#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include "Material.hpp"
#include <Shaders/PhongShader.hpp>

class Mesh {
private:
    GLuint vao = 0, vbo[3] = {0,0,0}, ebo = 0, elementCount = 0;

    std::vector<glm::vec3> *positions = nullptr, *normals = nullptr;
    std::vector<glm::vec2> *textCoords = nullptr;
    std::vector<glm::ivec3> *indices = nullptr;

    Mesh & operator =(Mesh const &) = delete;
public:
    static Mesh* build(aiMesh* mesh, const Material* material);

    Mesh(std::vector<glm::vec3> *positions, 
        std::vector<glm::vec3> *normals, 
        std::vector<glm::vec2> *textCoords,
        std::vector<glm::ivec3> *indices,
        const Material *material);
    ~Mesh();

    void load();
    bool isLoaded();
    void draw(PhongShader& shader);

    const Material *material;
};