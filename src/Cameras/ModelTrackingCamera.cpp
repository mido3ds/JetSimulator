#include "ModelTrackingCamera.hpp"
#include <glm/gtx/rotate_vector.hpp>

ModelTrackingCamera::ModelTrackingCamera(Model* target, float distance, float fovy, float aspect, float near, float far) 
    :Camera(fovy, aspect, near, far), target(target), distance(distance), app(App::getApp()) {
    lastMousePos = app->getMousePos();
} 

void ModelTrackingCamera::update(double dt) {
    // update yaw and pitch
    glm::vec2 mousePos = app->getMousePos();
    if (/*app->isKeyPressed(MOUSE_BUTTON_MIDDLE)*/true) {    
        yaw += YAW_SPEED * (mousePos.x - lastMousePos.x);
        pitch += PITCH_SPEED * (mousePos.y - lastMousePos.y);
    }
    lastMousePos = mousePos;

    // update view
    position = glm::vec3(-distance * glm::sin(yaw), distance * glm::sin(pitch), -distance * glm::cos(yaw));
    glm::vec3 normal = glm::rotate(glm::cross(target->up, target->front), yaw, target->up);
    glm::vec3 cameraUp = glm::rotate(target->up, pitch, normal);
    view = glm::lookAt(position, target->pos, cameraUp);
}