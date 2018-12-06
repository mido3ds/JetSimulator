#pragma once
#include "Camera.hpp"
#include <App/App.hpp>
#include <glm/glm.hpp>
#include <Scene/Model.hpp>

#define MOUSE_SENSITIVITY 0.01

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
    float yaw = -26.6927, pitch = 31.9759; 

    virtual void update(double dt) override;
};