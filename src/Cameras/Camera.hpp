#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up, 
            float /*radians*/ fovy, float aspect, float near, float far);
    
    glm::mat4 projection;
    glm::mat4 view;

    virtual void update(double dt);
};