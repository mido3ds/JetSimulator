#include "ModelTrackingCamera.hpp"
#include <glm/ext.hpp>

#define max(a, b) (a>b?a:b)
#define min(a, b) (a<b?a:b)

ModelTrackingCamera::ModelTrackingCamera(Model* target, float distance, float fovy, float aspect, float near, float far) 
    :Camera(fovy, aspect, near, far), target(target), distance(distance), app(App::getApp()) {
    lastMousePos = app->getMousePos();
} 

void ModelTrackingCamera::update(float dt) {
    // update yaw and pitch
    glm::vec2 mousePos = app->getMousePos();
    if (/*app->isKeyPressed(MOUSE_BUTTON_MIDDLE)*/true) {    
        yaw -= MOUSE_SENSITIVITY * (mousePos.x - lastMousePos.x);
        yaw = glm::wrapAngle(yaw);

        pitch -= MOUSE_SENSITIVITY * (mousePos.y - lastMousePos.y);
        pitch = glm::wrapAngle(pitch);

        if (pitch < glm::pi<float>()) pitch = min(pitch, 1.283f);
        else pitch = max(pitch, 5.036f);
    }
    lastMousePos = mousePos;

    // update view
    position = glm::vec3(
        glm::rotate(yaw, target->up) * 
        glm::rotate(pitch, target->right) * 
        glm::vec4(- distance * target->front, 1)
    );
    position += target->pos;

    view = glm::lookAt(position, target->pos, target->up);
}