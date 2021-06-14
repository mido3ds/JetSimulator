#include <Models/Missile.hpp>
#include <iostream>
#include <glm/gtx/io.hpp>

Missile::Missile(Node* node, float speed, float maxTime)
    :node(node), speed(speed*20), maxTime(maxTime) {
    assert(node && maxTime > 0 && speed > 0);
}

Missile::~Missile() {
	if (!attached())
		delete node;
}

void Missile::fire(float throwSpeed) {
    if (attached()) {
        node->removeFromParent();
        speed += throwSpeed*20;
    }
}

bool Missile::attached() {
    return node->attached();
}

bool Missile::exploded() {
    return maxTime <= 0;
}

void Missile::update(float dt) {
    if (!attached() && !exploded()) {
        glm::vec3 pos(node->transform[3]), front(node->transform[0]);
        pos += dt * speed * front;
        node->transform[3] = glm::vec4(pos, 1.0f);

        maxTime -= dt;

		//std::cout << pos << "\n";
    }
}

void Missile::draw(PhongShader* shader) {
    if (!attached() && !exploded()) {
        node->draw(*shader);
    }
}