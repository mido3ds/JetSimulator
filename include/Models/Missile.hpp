#pragma once
#include <Scene/Node.hpp>
#include <glm/glm.hpp>
#include <Shaders/PhongShader.hpp>
#include <memory>
#include <cassert>

using namespace std;

class Missile {
private: 
    unique_ptr<Node> node;
    float maxTime;
public:
    float speed;
    inline Missile(unique_ptr<Node> node, float maxTime, float speed)
        :node(move(node)), speed(speed), maxTime(maxTime) {
        assert(this->node && maxTime > 0 && speed > 0);
    }

    // TODO: remove this, and only use memory deletion
    bool exploded();

    void update(float dt);
    void render(PhongShader& shader);
};
