#ifndef VERTEX_H
#define VERTEX_H

#define GLM_FORCE_CXX11
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};

#endif