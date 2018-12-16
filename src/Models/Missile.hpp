#pragma once
#include <Scene/Node.hpp>
#include <glm/glm.hpp>
#include <Shaders/PhongShader.hpp>

class Missile {
private: 
    Node* node;
    float speed;
    float maxTime;
public:
    Missile(Node* node, float speed, float maxTime);
    ~Missile();

    void fire(float throwSpeed);
    bool attached();
    bool exploded();

    void update(float dt);
    void draw(PhongShader* shader);
};