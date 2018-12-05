#pragma once
#include "Camera.hpp"
#include <App/App.hpp>
#include <glm/glm.hpp>
#include <Scene/Model.hpp>

#define YAW_SPEED 1
#define PITCH_SPEED 1

class ModelTrackingCamera: public Camera {
private:
    glm::vec3 normal;
    App * const app;
    glm::vec2 lastMousePos;
public:
    ModelTrackingCamera(Model* target, float distance, 
                float fovy, float aspect, float near, float far);

    Model* target;
    float distance;
    float yaw = 0, pitch = 0; 

    virtual void update(double dt) override;
};