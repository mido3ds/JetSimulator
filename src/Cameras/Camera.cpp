#include <Cameras/Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up, 
            float /*radians*/ fovy, float aspect, float near, float far)
    :view(glm::lookAt(pos, target, up)), projection(glm::perspective(fovy, aspect, near, far)) {}

Camera::Camera(float fovy, float aspect, float near, float far):projection(glm::perspective(fovy, aspect, near, far)) {}

void Camera::update(float dt) {}