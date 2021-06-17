#include <Cameras/ModelTrackingCamera.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/transform.hpp>
#include <cassert>

#define max(a, b) (a>b?a:b)
#define min(a, b) (a<b?a:b)

ModelTrackingCamera::ModelTrackingCamera(Model* target, float minDist, float maxDist, float fovy, float aspect, float near, float far) 
    :Camera(fovy, aspect, near, far), target(target), 
        distance((maxDist+minDist)/2.0f), minDist(minDist), maxDist(maxDist), app(App::getApp()) {
    lastMousePos = app->getMousePos();
    assert(minDist <= maxDist);
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

    // update distance 
    distance += -app->getScroll();
    distance = glm::clamp(distance, minDist, maxDist);

    // update view
    position = glm::vec3(
        glm::rotate(yaw, target->up) * 
        glm::rotate(pitch, target->right) * 
        glm::vec4(- distance * target->front, 1)
    );
    position += target->pos;

    view = glm::lookAt(position, target->pos, target->up);
}
