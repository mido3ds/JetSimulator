#include <Models/Missile.hpp>
#include <iostream>
#include <glm/gtx/io.hpp>

bool Missile::exploded() {
    return maxTime <= 0;
}

// TODO: should accelerate
void Missile::update(float dt) {
    if (!exploded()) {
        glm::vec3 pos(node->transform[3]), front(node->transform[0]);
        pos += dt * speed * front;
        node->transform[3] = glm::vec4(pos, 1.0f);

        maxTime -= dt;
    }
}

void Missile::render(PhongShader& shader) {
    if (!exploded()) {
        node->render(shader);
    }
}
