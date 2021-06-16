#pragma once
#include <Scene/Node.hpp>
#include <glm/glm.hpp>
#include <Shaders/PhongShader.hpp>
#include <memory>

using namespace std;

class Missile {
private: 
    Node* node;
    float speed;
    float maxTime;
    
    shared_ptr<Node> workaround_node; // TODO: remove this hack
public:
    Missile(Node* node, float speed, float maxTime);

    void fire(float throwSpeed);
    bool attached();
    bool exploded();

    void update(float dt);
    void render(unique_ptr<PhongShader>& shader);
};
