#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Material.hpp"

class Mesh {
private:
    GLuint vao = 0, vbo[3] = {0,0,0}, ebo = 0, elementCount = 0;

    std::vector<glm::vec3> *positions = nullptr, *normals = nullptr, *textCoords = nullptr;
    std::vector<glm::ivec3> *indices = nullptr;

    Mesh & operator =(Mesh const &) = delete;
public:
    Mesh(std::vector<glm::vec3> *positions, 
        std::vector<glm::vec3> *normals, 
        std::vector<glm::vec3> *textCoords,
        std::vector<glm::ivec3> *indices,
        Material *material);
    ~Mesh();

    void load();
    bool isLoaded();
    void draw();

    const Material *material;
};