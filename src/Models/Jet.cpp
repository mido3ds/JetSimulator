#include "Jet.hpp"
#include <App/App.hpp>
#include <glm/ext.hpp>

Jet::Jet() :Model(JET_MODEL_PATH), app(App::getApp()) {}

void Jet::update(float dt) {
    // rotation
    if (app->isKeyPressed(KEY_W)) {
        pitch += ROTATE_SPEED * dt;
    } else if (app->isKeyPressed(KEY_S)) {
        pitch -= ROTATE_SPEED * dt;
    }
    if (app->isKeyPressed(KEY_E)) {
        yaw += ROTATE_SPEED * dt;
    } else if (app->isKeyPressed(KEY_Q)) {
        yaw -= ROTATE_SPEED * dt;
    }
    if (app->isKeyPressed(KEY_D)) {
        roll -= ROTATE_SPEED * dt;
    } else if (app->isKeyPressed(KEY_A)) {
        roll += ROTATE_SPEED * dt;
    }
    yaw = glm::wrapAngle(yaw);
    pitch = glm::wrapAngle(pitch);
    roll = glm::wrapAngle(roll);

    // translation
    if (app->isKeyPressed(KEY_UP)) {
        pos += dt * 30 * front;
    } else if (app->isKeyPressed(KEY_DOWN)) {
        pos -= dt * 30 * front;
    } 
    if (app->isKeyPressed(KEY_RIGHT)) {
        pos += dt * 30 * right;
    } else if (app->isKeyPressed(KEY_LEFT)) {
        pos -= dt * 30 * right;
    }

    // final transformation
    rootNode->transform = glm::translate(pos) * 
            glm::yawPitchRoll(0.0f, pitch, yaw) * 
            glm::rotate(roll, glm::vec3(0, 1, 0));

    // update unit vectors
    right = glm::vec3(rootNode->transform[0]);
    front = glm::vec3(rootNode->transform[1]);
    up = glm::vec3(rootNode->transform[2]);
}

void Jet::draw(PhongShader& shader) {
    rootNode->draw(shader);
}
