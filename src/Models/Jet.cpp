#include "Jet.hpp"
#include <App/App.hpp>
#include <glm/ext.hpp>

#define max(a, b) (a>b?a:b)
#define min(a, b) (a<b?a:b)

Jet::Jet() :Model(JET_MODEL_PATH), app(App::getApp()) {}

void Jet::update(float dt) {
    float dYaw=0,dPitch=0, dRoll=0;

    // rotation
    if (app->isKeyPressed(KEY_W)) {
        dPitch = +ROTATE_SPEED * dt;
    } else if (app->isKeyPressed(KEY_S)) {
        dPitch = -ROTATE_SPEED * dt;
    }
    if (app->isKeyPressed(KEY_D)) {
        dRoll = +ROTATE_SPEED * dt;
    } else if (app->isKeyPressed(KEY_A)) {
        dRoll = -ROTATE_SPEED * dt;
    }

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

    // transformation
    glm::mat3 rRoll(glm::rotate(dRoll, front));
    right = rRoll * right;
    glm::mat3 rPitch(glm::rotate(dPitch, right));
    up = rPitch * rRoll * up;
    glm::mat3 rYaw(glm::rotate(dYaw, up));
    right = rYaw * right;
    front = glm::cross(up, right);
    
    rootNode->transform = glm::mat4(
        right.x, right.y, right.z, 0.0f,
        front.x, front.y, front.z, 0.0f,
        up.x, up.y, up.z, 0.0f,
        pos.x, pos.y, pos.z, 1.0f
    );

    // wrap angles
    yaw = yaw+dYaw;
    pitch = glm::wrapAngle(pitch+dPitch);
    roll = glm::wrapAngle(roll+dRoll);
}
