#pragma once
#include <glm/glm.hpp>

struct DirLight {
    glm::vec3 /*normalized*/ dir; // light -> fragment
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight {
    glm::vec3 pos;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float quadratic, linear, constant; 
};

struct SpotLight {
    glm::vec3 pos;
    glm::vec3 /*normalized*/ dir; // light -> fragment
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float innerCone, outerCone;   
    float quadratic, linear, constant; 
};