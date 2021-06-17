#pragma once
#include <Scene/Node.hpp>
#include <glm/glm.hpp>
#include <Shaders/PhongShader.hpp>
#include <memory>
#include <cassert>

using namespace std;

class Missile {
private: 
    shared_ptr<Node> node;
    float maxTime;
public:
    float speed;
    inline Missile(shared_ptr<Node> node, float maxTime, float speed)
        :node(node), speed(speed), maxTime(maxTime) {
        assert(node && maxTime > 0 && speed > 0);
    }

    // TODO: remove this, and only use memory deletion
    bool exploded();

    void update(float dt);
    void render(unique_ptr<PhongShader>& shader);
};
