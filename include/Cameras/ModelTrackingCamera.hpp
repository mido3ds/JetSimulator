#pragma once
#include "Camera.hpp"
#include <glm/glm.hpp>
#include <Scene/Model.hpp>
#include <System.hpp>

#define MOUSE_SENSITIVITY 0.008

class ModelTrackingCamera: public Camera {
private:
    glm::vec3 normal;
    MousePos lastMousePos;
    const float minDist, maxDist;
public:
    ModelTrackingCamera(Model& target, float minDist, float maxDist, 
                float fovy, float aspect, float near, float far);

    Model* target;
    float distance;
    float yaw = 0, pitch = 5.8f; 

    virtual void update(float dt) override;
};
