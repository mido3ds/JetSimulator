#include "Jet.hpp"
#include <App/App.hpp>
#include <glm/ext.hpp>

#define max(a, b) (a>b?a:b)
#define min(a, b) (a<b?a:b)

Jet::Jet() :Model(JET_MODEL_PATH), app(App::getApp()) {}

void Jet::update(float dt) {
    yaw = 0;

    // rotation
    if (app->isKeyPressed(KEY_W)) {
        pitch += ROTATE_SPEED * dt;
    } else if (app->isKeyPressed(KEY_S)) {
        pitch -= ROTATE_SPEED * dt;
    }
    if (app->isKeyPressed(KEY_E)) {
        yaw += YAW_MAX;
    } else if (app->isKeyPressed(KEY_Q)) {
        yaw -= YAW_MAX;
    }
    if (app->isKeyPressed(KEY_D)) {
        roll += ROTATE_SPEED * dt;
    } else if (app->isKeyPressed(KEY_A)) {
        roll -= ROTATE_SPEED * dt;
    }
    yaw = glm::wrapAngle(yaw);
    pitch = glm::wrapAngle(pitch);
    roll = glm::wrapAngle(roll);

    if (yaw < glm::pi<float>()) yaw = min(yaw, 0.0713778f);
    else yaw = max(yaw, 6.23377f);

    // translation
    if (app->isKeyPressed(KEY_UP)) {
        pos += dt * 300 * front;
    } else if (app->isKeyPressed(KEY_DOWN)) {
        pos -= dt * 300 * front;
    } 
    if (app->isKeyPressed(KEY_RIGHT)) {
        pos += dt * 300 * right;
    } else if (app->isKeyPressed(KEY_LEFT)) {
        pos -= dt * 300 * right;
    }

    // final transformation
    glm::mat3 rRoll(glm::rotate(roll, glm::vec3(0, 1, 0)));
    right = rRoll * glm::vec3(1, 0, 0);
    glm::mat3 rPitch(glm::rotate(pitch, right));
    up = rPitch * rRoll * glm::vec3(0, 0, 1);
    glm::mat3 rYaw(glm::rotate(yaw, up));
    rootNode->transform = glm::translate(pos) * glm::mat4(rYaw * rPitch * rRoll);
    front = glm::vec3(rootNode->transform[1]);
}
